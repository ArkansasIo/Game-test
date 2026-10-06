/*
 * strings.js
 *
 * Defines and exports a series of namespaced strings meant to be preformatted
 * and transpiled into C files for inclusion into the ROM.
 */

/**
 * Namespaces to export.
 */
const namespaces = {};

/**
 * Adds a string namespace. Each namespace generates a separate banked C file
 * that defines the formatted string constants.
 * @param {string} name Name for the namespace.
 * @param {number} bank ROM bank in which to write the strings.
 * @param {object} strings Key value pairs of names and string values.
 */
function addNamespace(name, bank, strings) {
  namespaces[name] = { bank, strings };
}

addNamespace('misc', 0, {
  'empty': 'EMPTY...',
  'physical': '',
  'magical': 'magical',
  'earth': 'earth',
  'water': 'water',
  'air': 'air',
  'fire': 'fire',
  'light': 'light',
  'dark': 'dark',
  'potion':        'Potion',
  'remedy':        'Remedy',
  'ether':         'Ether ',
  'atk_up_potion': "ATK&  ",
  'def_up_potion': 'DEF&  ',
  'elixir':        'Elixir',
  'regen_pot':     'Regen ',
  'haste_pot':     'Haste ',
  'dummy': 'Dummy',
  'kobold': 'Kobold',
  'goblin': 'Goblin',
  'zombie': 'Zombie',
  'bugbear': 'Bugbear',
  'owlbear': 'Owlbear',
  'gelatinous_cube': 'G.Cube',
  'displacer_beast': 'D.Beast',
  'will_o_wisp': 'W.O.Wisp',
  'death_knight': 'D.Knight',
  'mind_flayer': 'M.Flayer',
  'beholder': 'Beholder',
  'dragon': 'Dragon',
  'druid_short':    'DRU',
  'fighter_short':  'FTR',
  'monk_short':     'MNK',
  'sorcerer_short': 'SORC',
  'monster_miss_evaded': 'But you evade!',
})

addNamespace('ability', 31, {
  // Druid Attack & Abilities
  'druid_poison_spray':   'Poison Spray ',
  'druid_cure_wounds':    'Cure Wounds  ',
  'druid_bark_skin':      'Bark Skin    ',
  'druid_lightning':      'Lightning    ',
  'druid_heal':           'Heal         ',
  'druid_insect_plague':  'Insect Plague',
  'druid_regen':          'Regenerate   ',
  // Fighter Attack & Abilities
  'fighter_attack':       'Melee Attack ',
  'fighter_second_wind':  'Second Wind  ',
  'fighter_action_surge': 'Action Surge ',
  'fighter_cleave':       'Cleave       ',
  'fighter_trip_attack':  'Trip Attack  ',
  'fighter_menace':       'Menace       ',
  'fighter_indomitable':  'Indomitable  ',
  // Monk Attack & Abilities
  'monk_attack':          'Unarmed Atk. ',
  'monk_evasion':         'Evasion      ',
  'monk_open_palm':       'Open Palm    ',
  'monk_still_mind':      'Still Mind   ',
  'monk_flurry':          'Flurry       ',
  'monk_diamond_body':    'Diamond Body ',
  'monk_quivering_palm':  'Quiver. Palm ',
  // Sorcerer Attack & Ability
  'sorc_attack':          'Magic Missile',
  'sorc_darkness':        'Darkness     ',
  'sorc_fireball':        'Fireball     ',
  'sorc_haste':           'Haste        ',
  'sorc_sleetstorm':      'Sleetstorm   ',
  'sorc_disintegrate':    'Disintegrate ',
  'sorc_wild_magic':      'Wild Magic   ',
  // Necromancer Attack & Abilities
  'necro_attack':         'Drain Touch  ',
  'necro_drain_life':     'Drain Life   ',
  'necro_bone_armor':     'Bone Armor   ',
  'necro_raise_dead':     'Raise Dead   ',
  'necro_curse':          'Curse        ',
  'necro_soul_harvest':   'Soul Harvest ',
  'necro_army_dead':      'Army of Dead ',
  // Rune Paladin Attack & Abilities
  'pally_attack':         'Warhammer    ',
  'pally_lay_hands':      'Lay on Hands ',
  'pally_shield_faith':   'Shield Faith ',
  'pally_smite':          'Divine Smite ',
  'pally_consecrate':     'Consecrate   ',
  'pally_divine_shield':  'Divine Shield',
  'pally_hammer_god':     'Hammer of God',
  // Shadow Assassin Attack & Abilities
  'assassin_attack':      'Dagger Strike',
  'assassin_shadow_step': 'Shadow Step  ',
  'assassin_poison_blade':'Poison Blade ',
  'assassin_assassinate': 'Assassinate  ',
  'assassin_vanish':      'Vanish       ',
  'assassin_death_mark':  'Death Mark   ',
  'assassin_thousand':    'Thousand Cuts',
  // Stormcaller Attack & Abilities
  'storm_attack':         'Spark        ',
  'storm_static_charge':  'Static Charge',
  'storm_chain_light':    'Chain Light. ',
  'storm_storm_shield':   'Storm Shield ',
  'storm_light_storm':    'Lightning Stm',
  'storm_thunder_god':    'Thunder God  ',
});

addNamespace('battle', 3, {
  'monster_scared_frozen': '%monster %c shivers in fear...',
  'monster_paralyzed': "%monster %c can't move!",
  'monster_poison_death': '%monster %c succumbs to the poison!',
  'monster_confuse_attack_self': 'Confused, %monster %c attacks itself!',
  'monster_confuse_attack_other': 'Confused, %monster %c attacks any ally!',
  'monster_confuse_stupor': '%monster %c stares aimlessly.',
  'monster_lies_prone': '%monster %c lies prone!',
  'monster_gets_up': '%monster %c gets up.',
  'monster_ice_slip': '%monster %c slips on the ice!',
  'player_flee_attempt': 'You attempt to flee...',
  'player_flee_success': 'And get away!',
  'player_flee_failure': 'But are blocked!',
  'player_scared': 'You shiver with fear!',
  'player_prone': 'You lie prone!',
  'player_get_up': 'You get up!',
  'player_paralyzed': 'You are paralyzed and cannot move!',
  'player_confused_attack': 'You deal %damage damage to yourself!',
  'player_confused_mumble': 'You mumble some gibberish and giggle a little.',
  'victory': 'Victory! You gain %exp XP!',
  'victory_no_xp': 'Victory! But you gain no XP...',
  'level_up': 'LEVEL UP! Welcome\nto level %level!\n\nHP+%cP restored!',
})

addNamespace('player', 4, {
  // Common
  'miss': 'But you miss!',
  'hit': 'You deal %damage damage!',
  'hit_immune': "They're completely immune!",
  'hit_resist': "They resist your attack, only %damage damage...",
  'hit_vuln': "SUPER EFFECTIVE %damage damage!",
  'hit_crit': 'CRITICAL HIT! You deal %damage damage!',
  'heal_hp': 'You heal %damage HP.',
  'heal_crit': 'CRITICAL! You heal a whopping %damage HP!',
  'heal_fumble': 'You only heal a measly %damage HP.',
  'miss_all': 'A COMPLETE WHIFF.',
  // Druid abilities
  'poison_spray': 'Poison gas erupts from your palm!',
  'cure_wounds': "You're enveloped in blue light...",
  'bark_skin': 'Your skin grows hard as wood.',
  'lightning': 'Bolts of lighting fall!',
  'heal': 'Radiant green light descends...',
  'heal_complete': "You're fully healed!",
  'insect_plague': 'Locusts swarm!',
  'regen': 'You surge with vitality!',
  // Fighter abilities
  'fighter_attack': 'You rush forward!',
  'second_wind': 'You catch your breath...',
  'action_surge': 'You surge forth!',
  'cleave': 'You cleave through your enemies!',
  'trip_attack': 'You sweep your legs low...',
  'trip_attack_hit': 'You topple your foe!',
  'menace': 'You growl menacingly!',
  'indomitable': 'You feel invincible!',
  // Monk abilities
  'monk_attack': 'You strike with your fists!',
  'monk_evasion': 'You feel light on your feet!',
  'monk_open_palm': 'You strike with an open palm!',
  'monk_open_palm_trip': 'You trip %monster %c!',
  'monk_still_mind': 'You become one with the multiverse...',
  'monk_still_mind_post': 'And are healed of all ill effects!',
  'monk_flurry_of_blows': 'You attack with a flurry of blows!',
  'monk_diamond_body': 'You become tough as diamond.',
  'monk_quivering_palm': 'You attack their very essence!',
  'monk_quivering_kill': 'And end them.',
  // Sorcerer abilities
  'sorc_magic_missile_one': 'You fire a magic missile!',
  'sorc_magic_missile': 'You fire %1u magic missiles!',
  'sorc_darkness': 'You enshroud your enemies in darkness!',
  'sorc_fireball': 'EXPLOSION!',
  'sorc_haste': 'You speed up, a lot.',
  'sorc_sleetstorm': 'Sleet rains down!',
  'sorc_disintegrate': 'You send forth a ray of DEATH!',
  'sorc_disintegrate_kill': 'And they are no more.',
  'sorc_wild_magic': 'You let loose a storm of magic!',
  'sorc_wild_magic_fizzle': 'But it fizzles.',
  'sorc_wild_magic_fireball': 'And a fireball goes flying!',
  'sorc_wild_magic_sleetstorm': 'And a sleetstorm descends!',
  // `damage_monster` strings (in player.c)
  'displacer_beast_phase': 'They phase out and evade the attack!',
  'deathknight_revive': 'The deathknight falls, but then revives!',
  // Necromancer abilities
  'necro_attack': 'You reach out with a decaying hand!',
  'necro_drain_life': 'You tear the life from your foe!',
  'necro_bone_armor': 'Plates of bone knit around you.',
  'necro_raise_dead': 'The dead rise to serve you!',
  'necro_curse': 'You lay a withering curse upon them!',
  'necro_soul_harvest': 'You reap the souls of the fallen!',
  'necro_army_dead': 'An ARMY of the dead answers your call!',
  // Rune Paladin abilities
  'pally_attack': 'You swing your warhammer!',
  'pally_lay_hands': 'Holy light mends your wounds.',
  'pally_shield_faith': 'A shimmering ward surrounds you.',
  'pally_smite': 'You call down a DIVINE SMITE!',
  'pally_consecrate': 'The ground is consecrated in holy fire!',
  'pally_divine_shield': 'You are shielded from all harm!',
  'pally_hammer_god': 'You summon the HAMMER OF GOD!',
  // Shadow Assassin abilities
  'assassin_attack': 'You slip your dagger between their ribs!',
  'assassin_shadow_step': 'You melt into the shadows.',
  'assassin_poison_blade': 'Your blade drips with venom!',
  'assassin_assassinate': 'You strike for the killing blow!',
  'assassin_vanish': 'You vanish from sight!',
  'assassin_death_mark': 'You mark them for death!',
  'assassin_thousand_cuts': 'A THOUSAND CUTS rain down!',
  // Stormcaller abilities
  'storm_attack': 'You hurl a crackling spark!',
  'storm_static_charge': 'Static builds around you!',
  'storm_chain_light': 'Lightning arcs between your foes!',
  'storm_storm_shield': 'A storm wreathes your body!',
  'storm_light_storm': 'The heavens open with lightning!',
  'storm_thunder_god': 'You become the THUNDER GOD!',
});

addNamespace('menu', 31, {
  'root': 'Menu',
  'items': 'Items',
  'equip': 'Equip',
  'skills': 'Skills',
  'status': 'Status',
  'party': 'Party',
  'quests': 'Quests',
  'system': 'System',

  'no_items': 'You have no items.',
  'no_skills': 'You know no skills.',
  'none': 'None',
  'alone': 'Travelling alone',

  'row_level': 'Level',
  'row_hp': 'HP',
  'row_max_hp': 'Max HP',
  'row_sp': 'SP',
  'row_atk': 'Attack',
  'row_def': 'Defense',
  'row_agl': 'Agility',
  'row_exp': 'Experience',
  'row_act': 'Act',
  'row_chapter': 'Chapter',
  'row_weapon': 'Weapon',
  'row_armor': 'Armor',
  'row_relic': 'Relic',
  'row_alone': 'No companions.',

  'save': 'Save',
  'options': 'Options',
  'quit': 'Quit',
});

addNamespace('story', 31, {
  // ---------------------------------------------------------------------------
  // ACT I - The Waking Dark
  // ---------------------------------------------------------------------------
  'ch1_title': 'Chapter 1 - Cold Open',
  'ch1_b1': 'You wake on cold stone. You do not remember lying down.',
  'ch1_b2': 'The torch beside you is already burning. You did not light it.',
  'ch1_b3': 'Somewhere below, something enormous is breathing.',

  'ch2_title': 'Chapter 2 - The First Hollow',
  'ch2_b1': 'A figure stands in the corridor ahead. It does not move toward you.',
  'ch2_b2': '"I left the kettle on," it says. "I left the kettle on."',
  'ch2_b3': 'It has said this for a very long time.',

  'ch3_title': 'Chapter 3 - Stone and Silence',
  'ch3_b1': 'The corridor opens into a hall that should not fit inside the hill.',
  'ch3_b2': 'Above, on a ledge, someone is watching you. They do not wave.',
  'ch3_b3': 'You take the torch and go down.',

  'ch4_title': 'Chapter 4 - The Ledger',
  'ch4_b1': 'A journal lies open on a desk, as if the reader stepped away.',
  'ch4_b2': 'The handwriting is yours. The entries are dated after today.',
  'ch4_b3': 'The last line reads: DO NOT GO DOWN. There are more lines below it.',

  'ch5_title': 'Chapter 5 - Ash Gate',
  'ch5_b1': 'An outpost clings to the wall of the shaft. Ashen banners.',
  'ch5_b2': 'A Warden bars the way. "Nobody comes up from below," she says.',
  'ch5_b3': '"So tell me how you got there."',

  'ch6_title': 'Chapter 6 - What the Torch Knows',
  'ch6_b1': 'The torch leans toward a sealed door. It has never done that before.',
  'ch6_b2': 'The Wardens exchange a look. One of them leaves without a word.',
  'ch6_b3': 'Nobody explains. Nobody ever explains.',

  'ch7_title': 'Chapter 7 - The Second Name',
  'ch7_b1': 'An old Warden stops mid-sentence and stares at you.',
  'ch7_b2': '"Vessel," she says. "You came back."',
  'ch7_b3': 'You have never heard that name. It fits you anyway.',

  'ch8_title': 'Chapter 8 - Down',
  'ch8_b1': 'The descent is long. The walls stop being walls.',
  'ch8_b2': 'Staircases loop back into themselves. Doors open onto other doors.',
  'ch8_b3': 'The Labyrinth is not built. It is arranged, by something.',

  'ch9_title': 'Chapter 9 - The Choir Below',
  'ch9_b1': 'Through solid rock comes the ring of hammers, thousands of them.',
  'ch9_b2': 'The Iron Choir is still working. They have never stopped.',
  'ch9_b3': 'Whatever they are making, they are making a great deal of it.',

  'ch10_title': 'Chapter 10 - The Eyes',
  'ch10_b1': 'The floor ends. There is no bottom, only distance.',
  'ch10_b2': 'Two lights open far below you. They are not lamps.',
  'ch10_b3': 'They are looking back.',

  // ---------------------------------------------------------------------------
  // ACT II - The Iron Choir
  // ---------------------------------------------------------------------------
  'ch11_title': 'Chapter 11 - The Foundry Door',
  'ch11_b1': 'The Choir does not turn you away. That is somehow worse.',
  'ch12_title': 'Chapter 12 - What They Sell',
  'ch12_b1': 'They will sell a blade to anyone. They are proud of this.',
  'ch13_title': 'Chapter 13 - The Ledger Again',
  'ch13_b1': 'A second journal. Same handwriting. Different year.',
  'ch14_title': 'Chapter 14 - Ash and Iron',
  'ch14_b1': 'The forges run on something that is not coal.',
  'ch15_title': 'Chapter 15 - The Tidebound',
  'ch15_b1': 'They come up at night to trade. They do not speak our language.',
  'ch16_title': 'Chapter 16 - What the Water Took',
  'ch16_b1': 'Below the flood line, the Labyrinth is older.',
  'ch17_title': 'Chapter 17 - The Hollow Choir',
  'ch17_b1': 'Hollows in the foundry. They work. They do not stop working.',
  'ch18_title': 'Chapter 18 - The Contract',
  'ch18_b1': 'Someone signed for all of this. There is a signature.',
  'ch19_title': 'Chapter 19 - Vessel',
  'ch19_b1': 'The name again. This time, someone says it to your face.',
  'ch20_title': 'Chapter 20 - The Second Eyes',
  'ch20_b1': 'They open again. Closer this time. Act II ends.',

  // ---------------------------------------------------------------------------
  // ACT III - The Sunken Crown
  // ---------------------------------------------------------------------------
  'ch21_title': 'Chapter 21 - The Flood Line',
  'ch21_b1': 'The water is warm. It should not be.',
  'ch22_title': 'Chapter 22 - The Drowned Archive',
  'ch22_b1': 'Records survive underwater better than above.',
  'ch23_title': 'Chapter 23 - The Five Crowns',
  'ch23_b1': 'Earth, water, air, fire, and the fifth. Nobody writes the fifth.',
  'ch24_title': 'Chapter 24 - What We Built Over',
  'ch24_b1': 'The Labyrinth is not the oldest thing here.',
  'ch25_title': 'Chapter 25 - The Concordance',
  'ch25_b1': 'A pact of five. Four signatures. One space.',
  'ch26_title': 'Chapter 26 - The Emberhand',
  'ch26_b1': 'They welcome you like they expected you.',
  'ch27_title': 'Chapter 27 - The Sermon',
  'ch27_b1': '"The Ruin is a birth," they say. "You are mourning a shell."',
  'ch28_title': 'Chapter 28 - The Deep Tidebound',
  'ch28_b1': 'They have been down here since before the Ruin. They remember.',
  'ch29_title': 'Chapter 29 - The Crown Below',
  'ch29_b1': 'It is under the water. It is under everything.',
  'ch30_title': 'Chapter 30 - The Third Eyes',
  'ch30_b1': 'Closer. Large enough now to see the shape of them.',

  // ---------------------------------------------------------------------------
  // ACT IV - The Nameless Throne
  // ---------------------------------------------------------------------------
  'ch31_title': 'Chapter 31 - The Throne Room',
  'ch31_b1': 'A throne with no name carved on it. It is not empty.',
  'ch32_title': 'Chapter 32 - Who Broke It',
  'ch32_b1': 'The answer is in the ledger. It always was.',
  'ch33_title': 'Chapter 33 - The Fifth Signature',
  'ch33_b1': 'The missing name is not a name. It is a title.',
  'ch34_title': 'Chapter 34 - Warden',
  'ch34_b1': 'Wardens do not guard the Labyrinth. Wardens are the Labyrinth.',
  'ch35_title': 'Chapter 35 - The Ashen Order Falls',
  'ch35_b1': 'They were keeping it closed. They were also keeping it fed.',
  'ch36_title': 'Chapter 36 - The Choir Stops',
  'ch36_b1': 'For the first time in a thousand years, the hammers are silent.',
  'ch37_title': 'Chapter 37 - The Tidebound Rise',
  'ch37_b1': 'They come up. All of them. All at once.',
  'ch38_title': 'Chapter 38 - The Hollow March',
  'ch38_b1': 'They walk toward the deep. They are not hostile. They are drawn.',
  'ch39_title': 'Chapter 39 - The Emberhand Burns',
  'ch39_b1': 'They got what they wanted. They do not look happy.',
  'ch40_title': 'Chapter 40 - The Fourth Eyes',
  'ch40_b1': 'You can see the whole of them now. Act IV ends.',

  // ---------------------------------------------------------------------------
  // ACT V - The Dragon\'s Dream
  // ---------------------------------------------------------------------------
  'ch41_title': 'Chapter 41 - The Dreamer',
  'ch41_b1': 'It is not sleeping. It has never been sleeping.',
  'ch42_title': 'Chapter 42 - Whose Dream',
  'ch42_b1': 'The corridors rearrange because something is turning over.',
  'ch43_title': 'Chapter 43 - The Last Warden',
  'ch43_b1': 'There is one left. It is you. It was always going to be you.',
  'ch44_title': 'Chapter 44 - The Ledger Closes',
  'ch44_b1': 'The final entry is dated today. You write it now.',
  'ch45_title': 'Chapter 45 - The Torch',
  'ch45_b1': 'You finally understand why it never went out.',
  'ch46_title': 'Chapter 46 - The Choice',
  'ch46_b1': 'Wake it. Or keep walking. There is no third door.',
  'ch47_title': 'Chapter 47 - The Deep Floor',
  'ch47_b1': 'The hundredth floor. There is no floor.',
  'ch48_title': 'Chapter 48 - The Name',
  'ch48_b1': 'You say the fifth name. It has been waiting.',
  'ch49_title': 'Chapter 49 - The Waking',
  'ch49_b1': 'The eyes close. The Labyrinth settles. Something else opens.',
  'ch50_title': 'Chapter 50 - The Dream Continues',
  'ch50_b1': 'You wake on cold stone. You do not remember lying down.',
  'ch50_b2': 'The torch beside you is already burning.',
});

addNamespace('maps', 2, {
  'world_wilderness': 'Wilderness',
  'world_village': 'Village',
  'world_town': 'Town',
  'world_city': 'City',
  'world_dungeon': 'Dungeon',
  'world_ocean': 'Ocean',
  'world_mountain': 'Mountains',
  'chest_locked': "The chest is locked.",
  'chest_open': "You opened the chest!",
  'chest_key_locked': "You need a magic key to unlock this chest...",
  'chest_unlock_key': "You unlock the chest with a magic key!",
  'get_magic_key': "You get a magic key!",
  'get_torch': "You find a torch!",
  'already_has_torch': "The chest is empty!",
  'lever_stuck': "It's stuck!",
  'lever_one_way': "Seems this lever was one and done.",
  'door_locked': 'The door is locked.',
  'door_locked_key': 'You need a magic key to unlock this door...',
  'door_unlock_key': 'You unlock the door with a magic key!',
  'sconce_lit_no_torch': 'The sconce burns brightly.',
  'sconce_no_torch': 'Hmm... how do you light this?',
  'sconce_torch_not_lit': 'Your torch lacks a flame.',
  'boss_not_yet': 'Get out of here, runt!',
});

addNamespace('floor_test', 2, {
  'metal_skull': 'This skull is so metal!',
  'glowing_eyes': 'A pair of glowing eyes peers back...',
  'click': 'You hear a click...',
  'creak': 'The other lever creaks.',
  'groan': 'The other lever groans.',
  'chest_click': 'The chest clicks.',
  'no_back': "You cannot return...",
  'door_opens': "The door opens!",
  'growl': "GROWL!",
  'healed': 'You are fully restored!',
})

addNamespace('floor1', 2, {
  'sign_monster_no_fire': 'Monsters fear fire.',
  'sign_empty_chest': "It's empty...",
  'sign_tunnel_cave_in': 'The tunnel has collapsed behind you!',
  'sign_skull_out_of_place': 'This skull seems out of place...',
  'sign_hidden_passage_hint': 'Check behind you...',
  'sign_missing_elite': 'A powerful foe once lived here.',
  'boss_defeated': "Yawp! You won't beat my friends below!",
});

addNamespace('floor2', 2, {
  'sign_items_room': 'Soon you will have to choose.',
  'sign_levers': 'Levers change many things.',
  'door_opens': 'Somewhere a door opens...',
  'elite_msg': "An adventurer? Come, let's test your mettle!",
  'boss_msg': 'SCREEEEEECH!',
});

addNamespace('floor3', 2, {
  'choose_wisely': 'Choose, but choose wisely...',
  'brains': 'BRAIINNNNSS...',
  'boss': 'Jiggle Jiggle... Jiggle...',
  'boss_not_yet': 'Jiggle?',
});

addNamespace('floor4', 2, {
  'elite_attack': 'KWAAAAAAHHH!',
  'boss': 'NyaAAAHHHH!',
  'boss_not_yet': 'Nya?',
});

addNamespace('floor5', 2, {
  'demands': 'The Dragon demands a rainbow...',
  'secrets': 'The walls have secrets...',
  'elite_attack': 'JIGGLE!',
  'boss': 'Come and meet DEATH!',
  'boss_not_yet': 'You are... unworthy.',
});

addNamespace('floor6', 2, {
  'elite_attack': 'BZZZTTT!',
  'boss': 'OH! What a delicious brain!',
  'boss_not_yet': 'You have yet to ripen...',
});

addNamespace('floor7', 2, {
  'riddle': 'Only those unseen may pass...',
  'elite_attack': 'HISSSSS!',
  'boss': 'STARING INTENSIFIES',
  'boss_not_yet': 'EYEBROW RAISES'
});

addNamespace('floor8', 2, {
  'boss': 'Finally, I have awaited this...',
  'elite': 'STARING EVEN MORE',
  'healing_mirror': 'You look in the mirror and your wounds vanish!',
  'healing_mirror_none': 'The mirror has lost its luster...',
});

addNamespace('floor_common', 2, {
  'growl': "GROWL!",
  'light_fires': "Light these fires to open this door!",
  'missing': "Something used to have been here...",
  'no_return': "There is no going back!",
  'steve_jobs': "It's so sad that Steve Jobs Died of Ligma...",
  'tbd': "Placeholders are a big no-no in game development!",
  'new_ability': "You get an ability!",
  'fight_me': "Fight Me!",
  'love': "I LOVE YOU!",
  'strange_wind': "You feel a strong breeze from the north.",
})

function grant_ability_str(name, cast=true) {
  return cast ?
    `You learn to cast ${name}!` :
    `You learn to use ${name}!`
}

addNamespace('gain_ability', 2, {
  'druid1': grant_ability_str('Bark Skin'),
  'druid2': grant_ability_str('Lightning'),
  'druid3': grant_ability_str('Heal'),
  'druid4': grant_ability_str('Insect Plague'),
  'druid5': grant_ability_str('Regenerate'),
  'fighter1': grant_ability_str('Action Surge', false),
  'fighter2': grant_ability_str('Cleave', false),
  'fighter3': grant_ability_str('Trip Attack', false),
  'fighter4': grant_ability_str('Menace', false),
  'fighter5': grant_ability_str('Indomitable', false),
  'monk1': grant_ability_str('Open Palm', false),
  'monk2': grant_ability_str('Still Mind', false),
  'monk3': grant_ability_str('Flurry', false),
  'monk4': grant_ability_str('Diamond Body', false),
  'monk5': grant_ability_str('Quivering Palm', false),
  'sorcerer1': grant_ability_str('Fireball'),
  'sorcerer2': grant_ability_str('Haste'),
  'sorcerer3': grant_ability_str('Sleetstorm'),
  'sorcerer4': grant_ability_str('Disintegrate'),
  'sorcerer5': grant_ability_str('Wild Magic'),
  'necromancer1': grant_ability_str('Drain Life'),
  'necromancer2': grant_ability_str('Bone Armor'),
  'necromancer3': grant_ability_str('Raise Dead'),
  'necromancer4': grant_ability_str('Curse'),
  'necromancer5': grant_ability_str('Army of the Dead'),
  'rune_paladin1': grant_ability_str('Lay on Hands', false),
  'rune_paladin2': grant_ability_str('Shield of Faith', false),
  'rune_paladin3': grant_ability_str('Divine Smite', false),
  'rune_paladin4': grant_ability_str('Consecrate', false),
  'rune_paladin5': grant_ability_str('Hammer of God', false),
  'shadow_assassin1': grant_ability_str('Shadow Step', false),
  'shadow_assassin2': grant_ability_str('Poison Blade', false),
  'shadow_assassin3': grant_ability_str('Assassinate', false),
  'shadow_assassin4': grant_ability_str('Vanish', false),
  'shadow_assassin5': grant_ability_str('Thousand Cuts', false),
  'stormcaller1': grant_ability_str('Static Charge'),
  'stormcaller2': grant_ability_str('Chain Lightning'),
  'stormcaller3': grant_ability_str('Storm Shield'),
  'stormcaller4': grant_ability_str('Lightning Storm'),
  'stormcaller5': grant_ability_str('Thunder God'),
});

addNamespace('chest_item', 2, {
  '2pot_1eth': 'You get 2 potions and an ether!',
  '1pot': 'You get a potion!',
  '1pots': 'You get a potions!',
  'haste_pot': 'You get a haste potion!',
  'regen_pot': 'You get a regen potion!',
  '3regen': 'You get 3 regens!',
  '2pots': 'You get 2 potions!',
  '1eth': 'You get an ether!',
  '1eths': 'You get an ethers!',
  '1remedy': 'You get a remedy!',
  '3potions': 'You get 3 potions!',
  '3ethers': 'You get 3 ethers!',
  '1elixir': 'You get an elixir!',
  '1atkup_1defup': 'You get an ATK& and DEF&!',
  '3elixirs': 'You get 3 elixirs!',
  '3haste': 'You get 3 haste potions!',
});

addNamespace('items', 3, {
  'use_potion': '%damage HP healed!',
  'use_ether_sp': '%damage SP restored!',
  'use_ether_mp': '%damage MP restored!',
  'use_remedy': 'You remedy what ails you!',
  'use_atkup': 'Your attack increases!',
  'use_defup': 'Your defense increases!',
  'use_elixir': 'You fully heal!',
  'use_regen': 'You begin regenerating!',
  'use_haste': 'The world slows down!',
  'use_failed': "The item didn't work!",
})

addNamespace('monster', 6, {
  'flee': '%monster %c makes a run for it...',
  'flee_failure': 'But they cannot get away!',
  'flee_success': 'And they get away!',
  'attack': '%monster %c attacks!',
  'miss': 'But they miss!',
  'magic_miss': 'But it has no effect!',
  'hit': 'You take %damage damage!',
  'hit_aspect': 'You take %damage %aspect damage!',
  'hit_immune': "But you're completely immune!",
  'hit_resist': 'You resist, only %damage damage',
  'hit_vuln': "It's SUPER BAD! %damage damage!",
  'hit_crit': 'CRITICAL HIT! You take %damage damage!',
  'hit_barkskin': 'Your barkskin protects you! %damage damage.',
  'does_nothing': '%monster %c does nothing.',
  'dummy_pre': 'Dummy %c stands still.',
  'dummy_post_heal': '"I will never die..."',
  // Kobold Specials
  'kobold_axe': 'Kobold %c raises a tiny axe...',
  'kobold_fire': 'Kobold %c spits a glob of fire...',
  'kobold_dazed': 'Kobold %c looks dazed...',
  'kobold_does_nothing': 'And does nothing!',
  'kobold_miss': 'But instead it falls over and hiccups!',
  'kobold_get_up': 'Kobold %c gets up.',
  // Goblin specials
  'goblin_nose_pick': 'Goblin %c picks its nose.',
  'goblin_attack': "Goblin %c swings a shortsword!",
  'goblin_acid_arrow': "Goblin %c shoots an acid arrow!",
  // Zombie specials
  'zombie_brains': 'Hrrnng... brains.',
  'zombie_bite_miss': "Zombie %c's bite barely misses!",
  'zombie_bite_hit': "Zombie %c bites and poisons you!",
  'zombie_slam': "Zombie %c swipes at you!",
  // Bugbear special
  'bugbear_for_hruggek': 'Bugbear %c screams "FOR HRUGGEK!"',
  'bugbear_for_hruggek_hit': 'You shiver with fear!',
  'bugbear_for_hruggek_miss': 'You are unimpressed.',
  'bugbear_javelin': 'Bugbear %c throws a javelin!',
  'bugbead_club': 'Bugbear %c swings a club!',
  // Owlbear special
  'owlbear_pounce': 'Owlbear %c pounces!',
  'owlbear_pounce_topple': 'You take %damage damage and fall prone!',
  'owlbear_pounce_miss': 'You barely jump out of the way!',
  'owlbear_multi': 'Owlbear %c tears at you with beak and claw!',
  'owlbear_beak': 'Owlbear %c nips at you!',
  // G. Cube specials
  'gcube_search': 'Gelatinous Cube %c sends out feelers...',
  'gcube_paralyze': 'You are engulfed and paralyzed!',
  'gcube_poison': 'You are engulfed and poisoned!',
  'gcube_engulf_fail': 'You dodge as Gelatinous Cube %c tries to engulf you!',
  'gcube_attack': 'Gelatinous Cube %c swipes at you!',
  // Displacer Beast specials
  'displacer_beast_tentacle': 'Displacer Beast %c strikes with its tentacles!',
  'displacer_beast_2hit': 'They hit twice for %damage damage!',
  'displacer_beast_1hit': 'They hit once for %damage damage!',
  'displacer_beast_miss': 'But they miss with both tentacles!',
  // Will-o-wisp specials
  'will_o_wisp_lightning': 'Will-o-wisp %c sends lightning forth...',
  'will_o_wisp_scare': 'Will-o-wisp %c passes through you!',
  'will_o_wisp_scare_hit': 'Terror fills your soul!',
  'will_o_wisp_scare_miss': 'But you hold fast!',
  'will_o_wisp_siphon': 'Will-o-wisp %c siphons your soul...',
  'will_o_wisp_siphon_hit': 'They steal %damage HP!',
  'will_o_wisp_hit': 'They shock you for %damage damage!',
  // Deathknight Specials
  'deathknight_attack': 'Death Knight %c swings their longsword...',
  'deathknight_hit1': 'They slash you for %damage damage!',
  'deathknight_hit2': 'They hit twice! %damage damage!',
  'deathknight_hellfire': 'Death Knight %c sends forth a hellfire orb!',
  'deathknight_hellfire_hit': 'Direct hit! You take %damage damage!',
  'deathknight_hellfire_miss': 'You dodge! But still take %damage damage!',
});

addNamespace('monster2', 7, {
  'does_nothing': '%monster %c does nothing.',
  'monster_miss': 'But they miss!',
  // Mindflayer special
  'mindflayer_mind_blast':        'Mind Flayer %c emits a wave of psychic energy!',
  'mindflayer_mind_blast_miss':   'But you resist!',
  'mindflayer_mind_blast_hit':    'You take %damage damage, and are confused!',
  'mindflayer_tentacle':          'Mind Flayer %c lashes out with its tentacles!',
  'mindflayer_extract_brain':     'Mind Flayer %c attempts to eat your brain!',
  'mindflayer_extract_brain_hit': 'Your brain is gobbled up!',
  // Beholder special
  'beholder_bite': 'Beholder %c chomps at you...',
  'beholder_shoot_ray': 'Beholder %c shoots a ray from an eyestalk...',
  'beholder_ray_paralyze': 'You take %damage damage and are paralyzed!',
  'beholder_ray_fear': 'You take %damage damage and you feel dread!',
  'beholder_ray_slow': 'You take %damage damage and you slow down!',
  'beholder_ray_necro': 'You take %damage damage and are poisoned!',
  'beholder_ray_trip': 'You take %damage damage and fall prone!',
  'beholder_ray_death': 'Your soul escapes your body...',
  'beholder_ray_miss': 'But you dodge the ray!',
  'beholder_bite_miss': 'But their bite misses!',
  'beholder_ray_resist': 'But you resist the ray!',
  // Dragon Specials
  'dragon_attack': 'Dragon %c swoops down and attacks!',
  'dragon_miss': 'But their attacks miss!',
  'dragon_hit_triple': 'They hit THREE TIMES for %damage damage!',
  'dragon_hit_double': 'They hit TWICE for %damage damage!',
  'dragon_hit_single': 'They hit for %damage damage!',
  'dragon_legendary_tail': 'Dragon %c sweeps its tail!',
  'dragon_legendary_tail_miss': 'But you dodge out of the way!',
  'dragon_legendary_wing': 'Dragon %c beats its wings!',
  'dragon_legendary_wing_miss': 'But you take cover!',
  'dragon_legendary_wing_hit': 'You are toppled and take %damage damage!',
  'dragon_fright': 'Dragon %c towers above you!',
  'dragon_fright_miss': 'But your resolve does not waiver!',
  'dragon_fright_hit': 'And you fear for your life!',
  'dragon_fire_breath': 'Dragon %c exhales a wave of fire!',
  'dragon_fire_breath_miss': 'You dodge, but still take %damage damage!',
})

addNamespace('credits', 1, {
  //           X123456789012345678X
  'story1_1': 'With a last gasp',
  'story1_2': 'and puff of smoke',
  'story1_3': 'The dragon was',
  'story1_4': 'Defeated...',

  'story2_1': 'The quest done,',
  'story2_2': 'the hero ascended',
  'story2_3': 'an ancient',
  'story2_4': 'staircase.',

  'story3_1': 'And at long last...',
  'story3_2': 'was free of the',
  'story3_3': 'The dungeon and',
  'story3_4': '   the dragon.',

  'developed_by': 'DEVELOPED BY',
  'programming': 'PROGRAMMING',
  'game_design': 'GAME DESIGN',
  'monster_art': 'MONSTER ART',
  'title_art': 'TITLE ART',
  'dungeon_art': 'DUNGEON ART',

  'arkansasio': 'ArkansasIo',
  'ryan': 'Ryan Richards',
  'tommy': 'Tommy',
  'mono': 'Mono',
  'ledu': 'Ledu',
  'patreon': 'SPECIAL THANKS',
  'nh_patreon': 'ArkansasIo Patreon',
  'nh_patreon2': 'patreon.com/',
  'nh_patreon3': '   ArkansasIo',

  'thank_you': 'Thank you',
  'for_playing': 'for playing!',
});

// Export the namespaces
module.exports = namespaces;
