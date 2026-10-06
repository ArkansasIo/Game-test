// Migrate the positional assets/tables.csv into per-table CSV files described
// by assets/schema.conf.
//
// The old format had one column per (stat, tier) pair, named `stat_c`, `stat_b`,
// `stat_a`, `stat_s`. The new format has one row per (level, tier) pair.
//
// Usage: node tools/migrate_tables.js
const fs = require('fs');
const path = require('path');

const OLD = 'assets/tables.csv';
const OUT_DIR = 'data';

// ---------------------------------------------------------------- schema parse

function parseSchema(file) {
  const text = fs.readFileSync(file, 'utf8');
  const tables = [];
  let cur = null;

  for (const raw of text.split(/\r?\n/)) {
    const line = raw.trim();
    if (!line || line.startsWith('#')) continue;

    const tableMatch = line.match(/^\[table:([a-z_]+)\]$/);
    if (tableMatch) {
      cur = { name: tableMatch[1], columns: [], tiers: null, rows: null, source: null };
      tables.push(cur);
      continue;
    }
    if (!cur) continue;

    const kv = line.match(/^(\w+)\s*=\s*(.+?)\s*(?:#.*)?$/);
    if (!kv) continue;
    const [, key, value] = kv;

    if (key === 'column') {
      const parts = value.split(':').map(s => s.trim());
      cur.columns.push({ name: parts[0], type: parts[1], doc: parts[2] || '' });
    } else if (key === 'tiers') {
      cur.tiers = value.split(',').map(s => s.trim());
    } else if (key === 'rows') {
      cur.rows = parseInt(value, 10);
    } else if (key === 'source') {
      cur.source = value;
    }
  }
  return tables;
}

// ------------------------------------------------------------------ old format

function readOld() {
  const lines = fs.readFileSync(OLD, 'utf8')
    .split(/\r?\n/)
    .filter(l => l.trim().length > 0);

  const names = lines[0].split(',');
  const types = lines[1].split(',');
  const rows = lines.slice(2);

  const columns = names.map((name, i) => ({ name, type: types[i], index: i }));
  return { columns, rows };
}

// Old names look like `player_hp_c`; the trailing letter is the tier.
function splitTier(name) {
  const m = name.match(/^(.*)_([abcs])$/);
  if (!m) return null;
  return { base: m[1], tier: m[2] };
}

// ---------------------------------------------------------------------- main

const schema = parseSchema('assets/schema.conf');
const old = readOld();

console.log(`schema declares ${schema.length} tables`);
console.log(`old csv has ${old.columns.length} columns, ${old.rows.length} rows`);

if (!fs.existsSync(OUT_DIR)) fs.mkdirSync(OUT_DIR, { recursive: true });

let written = 0;
const problems = [];

for (const table of schema) {
  // Work out which old columns feed this table.
  const isTiered = Array.isArray(table.tiers);

  // The value column is the one that is not the key and not the tier.
  const keyName = table.columns[0].name;
  const valueCols = table.columns.filter(c => c.name !== keyName && c.name !== 'tier');
  if (valueCols.length !== 1) {
    problems.push(`${table.name}: expected exactly one value column, found ${valueCols.length}`);
    continue;
  }
  const valueName = valueCols[0].name;

  const outLines = [];

  if (isTiered) {
    outLines.push(`${keyName},tier,${valueName}`);

    // Find the four old columns: <valueName>_<tier>.
    const byTier = {};
    for (const t of table.tiers) {
      const oldName = `${valueName}_${t}`;
      const col = old.columns.find(c => c.name === oldName);
      if (!col) { problems.push(`${table.name}: no old column '${oldName}'`); }
      else byTier[t] = col.index;
    }
    if (Object.keys(byTier).length !== table.tiers.length) continue;

    for (let level = 0; level < table.rows; level++) {
      const row = old.rows[level];
      if (!row) { problems.push(`${table.name}: old csv missing row ${level}`); break; }
      const cells = row.split(',');
      table.tiers.forEach((t, tierIndex) => {
        outLines.push(`${level},${tierIndex},${cells[byTier[t]]}`);
      });
    }
  } else {
    // Flat table: the old column is named exactly like the value column.
    const col = old.columns.find(c => c.name === valueName);
    if (!col) { problems.push(`${table.name}: no old column '${valueName}'`); continue; }

    outLines.push(`${keyName},${valueName}`);
    for (let level = 0; level < table.rows; level++) {
      const row = old.rows[level];
      if (!row) { problems.push(`${table.name}: old csv missing row ${level}`); break; }
      const cells = row.split(',');
      outLines.push(`${level},${cells[col.index]}`);
    }
  }

  const outPath = table.source || path.join(OUT_DIR, `${table.name}.csv`);
  fs.writeFileSync(outPath, outLines.join('\n') + '\n', 'utf8');
  console.log(`wrote ${outPath} (${outLines.length - 1} data rows)`);
  written++;
}

console.log(`\n${written} file(s) written`);
if (problems.length) {
  console.log('\nproblems:');
  problems.forEach(p => console.log('  ' + p));
  process.exit(1);
}
