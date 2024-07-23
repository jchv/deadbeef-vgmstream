static const char* extension_list[] = {
    //"", /* vgmstream can play extensionless files too, but plugins must accept them manually */

    "208",
    "2dx9",
    "3do",
    "3ds", //txth/reserved [F1 2011 (3DS)] 
    "4", //for Game.com audio
    "8", //txth/reserved [Gungage (PS1)]
    "800",
    "9tav",

    "a3c", //txth/reserved [Puyo Puyo 20th Anniversary (PSP)]
    //"aac", //common
    "aa3", //FFmpeg/not parsed (ATRAC3/ATRAC3PLUS/MP3/LPCM/WMA)
    "aax",
    "abc", //txth/reserved [Find My Own Way (PS2) tech demo]
    "abk",
    //"ac3", //common, FFmpeg/not parsed (AC3)
    "acb",
    "acm",
    "acx",
    "ad", //txth/reserved [Xenosaga Freaks (PS2)]
    "adc", //txth/reserved [Tomb Raider The Last Revelation (DC), Tomb Raider Chronicles (DC)]
    "adm",
    "adp",
    "adpcm",
    "adpcmx",
    "ads",
    "adw",
    "adx",
    "afc",
    "afs2",
    "agsc",
    "ahx",
    "ahv",
    "ai",
    //"aif", //common
    "aifc", //common?
    //"aiff", //common
    "aix",
    "akb",
    "al", //txth/raw [Dominions 3 - The Awakening (PC)]
    "al2", //txth/raw [Conquest of Elysium 3 (PC)]
    "amb",
    "ams", //txth/reserved [Super Dragon Ball Z (PS2) ELF names]
    "amx",
    "an2",
    "ao",
    "ap",
    "apc",
    "as4",
    "asbin",
    "asd",
    "asf",
    "asr",
    "ass",
    "ast",
    "at3",
    "at9",
    "atsl",
    "atsl3",
    "atsl4",
    "atslx",
    "atx",
    "aud",
    "audio", //txth/reserved [Grimm Echoes (Android)]
    "audio_data",
    "aus",
    "awa", //txth/reserved [Missing Parts Side A (PS2)]
    "awb",
    "awc",
    "awd",

    "b1s", //txth/reserved [7 Wonders of the Ancient World (PS2)]
    "baf",
    "baka",
    "bank",
    "bar",
    "bcstm",
    "bcwav",
    "bcv", //txth/reserved [The Bigs (PSP)]
    "bfstm",
    "bfwav",
    "bg00",
    "bgm",
    "bgw",
    "bigrp",
    "bik",
    "bika", //fake extension for .bik (to be removed)
    "bik2",
    "binka", //FFmpeg/not parsed (BINK AUDIO)
    //"bin", //common
    "bk2",
    "bkr",  //txth/reserved [P.N.03 (GC), Viewtiful Joe (GC)]
    "blk",
    "bmdx", //fake extension (to be removed?)
    "bms",
    "bnk",
    "bnm",
    "bns",
    "bnsf",
    "bo2",
    "brstm",
    "brstmspm",
    "brwav",
    "brwsd", //fake extension for RWSD (non-format)
    "bsnd",
    "btsnd",
    "bvg",
    "bwav",

    "cads",
    "caf",
    "cat",
    "cbd2",
    "cbx",
    "cd",
    "cfn", //fake extension for CAF (renamed, to be removed?)
    "chd", //txth/reserved [Donkey Konga (GC), Star Fox Assault (GC)]
    "chk",
    "ckb",
    "ckd",
    "cks",
    "cnk",
    "cpk",
    "cps",
    "csa", //txth/reserved [LEGO Racers 2 (PS2)]
    "csb",
    "csmp",
    "cvs", //txth/reserved [Aladdin in Nasira's Revenge (PS1)]
    "cwav",
    "cxs",

    "d2", //txth/reserved [Dodonpachi Dai-Ou-Jou (PS2)]
    "da",
    //"dat", //common
    "data",
    "dax",
    "dbm",
    "dct",
    "dcs",
    "ddsp",
    "de2",
    "dec",
    "dic",
    "diva",
    "dmsg", //fake extension/header id for .sgt (to be removed)
    "ds2", //txth/reserved [Star Wars Bounty Hunter (GC)]
    "dsb",
    "dsf",
    "dsp",
    "dspw",
    "dtk",
    "dvi",
    "dyx", //txth/reserved [Shrek 4 (iOS)]

    "e4x",
    "eam",
    "eas",
    "eda", //txth/reserved [Project Eden (PS2)]
    "emff", //fake extension for .mul (to be removed)
    "enm",
    "eno",
    "ens",
    "esf",
    "exa",
    "ezw",

    "fag",
    "fcb", //FFmpeg/not parsed (BINK AUDIO)
    "fda",
    "filp",
    "fish",
    //"flac", //common
    "flx",
    "fsb",
    "fsv",
    "fwav",
    "fwse",

    "g1l",
    "gbts",
    "gca",
    "gcm",
    "gcub",
    "gcw",
    "ged",
    "genh",
    "gin",
    "gmd",  //txth/semi [High Voltage games: Charlie and the Chocolate Factory (GC), Zathura (GC)]
    "gms",
    "grn",
    "gsf",
    "gsp",
    "gtd",
    "gwb",
    "gwm",

    "h4m",
    "hab",
    "hca",
    "hd3",
    "hdr",
    "hdt",
    "his",
    "hps",
    "hsf",
    "hvqm",
    "hwx", //txth/reserved [Star Wars Episode III (Xbox)]
    "hx2",
    "hx3",
    "hxc",
    "hxd",
    "hxg",
    "hxx",
    "hwas",
    "hwb",
    "hwd",

    "iab",
    "iadp",
    "idmsf",
    "idsp",
    "idvi", //fake extension/header id for .pcm (renamed, to be removed)
    "idwav",
    "idx",
    "idxma",
    "ifs",
    "ikm",
    "ild",
    "ilf", //txth/reserved [Madden NFL 98 (PS1)]
    "ilv", //txth/reserved [Star Wars Episode III (PS2)]
    "ima",
    "imc",
    "imf",
    "imx",
    "int",
    "is14",
    "isb",
    "isd",
    "isws",
    "itl",
    "ivaud",
    "ivag",
    "ivb",
    "ivs", //txth/reserved [Burnout 2 (PS2)]

    "joe",
    "jstm",

    "kat",
    "kces",
    "kcey", //fake extension/header id for .pcm (renamed, to be removed)
    "km9",
    "kma",  //txth/reserved [Dynasty Warriors 7: Empires (PS3)]
    "kmx",
    "kovs", //fake extension/header id for .kvs
    "kno",
    "kns",
    "koe",
    "kraw",
    "ktac",
    "ktsl2asbin",
    "ktss", //fake extension/header id for .kns
    "kvs",
    "kwa",

    "l",
    "l00", //txth/reserved [Disney's Dinosaur (PS2)]
    "laac", //fake extension for .aac (tri-Ace)
    "ladpcm", //not fake
    "laif", //fake extension for .aif (various)
    "laiff", //fake extension for .aiff
    "laifc", //fake extension for .aifc
    "lac3", //fake extension for .ac3, FFmpeg/not parsed
    "lasf", //fake extension for .asf (various)
    "lbin", //fake extension for .bin (various)
    "ldat", //fake extension for .dat
    "ldt",
    "lep",
    "lflac", //fake extension for .flac, FFmpeg/not parsed
    "lin",
    "lm0",
    "lm1",
    "lm2",
    "lm3",
    "lm4",
    "lm5",
    "lm6",
    "lm7",
    "lmp2", //fake extension for .mp2, FFmpeg/not parsed
    "lmp3", //fake extension for .mp3, FFmpeg/not parsed
    "lmp4", //fake extension for .mp4
    "lmpc", //fake extension for .mpc, FFmpeg/not parsed
    "logg", //fake extension for .ogg
    "lopus", //fake extension for .opus, used by LOPU too
    "lp",
    "lpcm",
    "lpk",
    "lps",
    "lrmh",
    "lse",
    "lsf",
    "lstm", //fake extension for .stm
    "lwav", //fake extension for .wav
    "lwd",
    "lwma", //fake extension for .wma, FFmpeg/not parsed

    "mab",
    "mad",
    "map",
    "mc3",
    "mca",
    "mcadpcm",
    "mcg",
    "mds",
    "mdsp",
    "med",
    "mjb",
    "mi4", //fake extension for .mib (renamed, to be removed)
    "mib",
    "mic",
    "mnstr",
    "mogg",
    //"m4a", //common
    //"m4v", //common
    //"mov", //common
    "move",
    //"mp+", //common [Moonshine Runners (PC)]
    //"mp2", //common
    //"mp3", //common
    //"mp4", //common
    //"mpc", //common
    "mpdsp",
    "mpds",
    "mpf",
    "mps", //txth/reserved [Scandal (PS2)]
    "ms",
    "msa",
    "msb",
    "msd",
    "mse",
    "msf",
    "mss",
    "msv",
    "msvp", //fake extension/header id for .msv
    "msx",
    "mta2",
    "mtaf",
    "mtt", //txth/reserved [Splinter Cell: Pandora Tomorrow (PS2)]
    "mul",
    "mups",
    "mus",
    "musc",
    "musx",
    "mvb", //txth/reserved [Porsche Challenge (PS1)]
    "mwa", //txth/reserved [Fatal Frame (Xbox)]
    "mwv",
    "mxst",
    "myspd",

    "n64",
    "naac",
    "nds",
    "ndp", //fake extension/header id for .nds
    "nlsd",
    "no",
    "nop",
    "nps",
    "npsf", //fake extension/header id for .nps (in bigfiles)
    "nsa",
    "nsopus",
    "nub",
    "nub2",
    "nus3audio",
    "nus3bank",
    "nwa",
    "nwav",
    "nxa",
    "nxopus",

    //"ogg", //common
    "ogg_",
    "ogl",
    "ogv",
    "oma", //FFmpeg/not parsed (ATRAC3/ATRAC3PLUS/MP3/LPCM/WMA)
    "omu",
    "opu",
    //"opus", //common
    "opusx",
    "otm",
    "oto", //txth/reserved [Vampire Savior (SAT)]
    "ovb", //txth/semi [namCollection: Tekken (PS2), Tekken 5: Tekken 1-3 (PS2)]

    "p04", //txth/reserved [Psychic Force 2012 (DC), Skies of Arcadia (DC)]
    "p08", //txth/reserved [SoulCalibur (DC)]
    "p16", //txth/reserved [Astal (SAT)]
    "p1d", //txth/reserved [Farming Simulator 18 (3DS)]
    "p2a", //txth/reserved [Thunderhawk Operation Phoenix (PS2)]
    "p2bt",
    "p3d",
    "past",
    "pcm",
    "pdt",
    "pk",
    "pona",
    "pos",
    "ps3",
    "psb",
    "psf",
    "psh", //fake extension for .vsv (to be removed)
    "psn",
    "pwb",

    "r",
    "rac", //txth/reserved [Manhunt (Xbox)]
    "rad",
    "rak",
    "ras",
    "raw", //txth/reserved [Madden NHL 97 (PC)-pcm8u]
    "rda", //FFmpeg/reserved [Rhythm Destruction (PC)]
    "res", //txth/reserved [Spider-Man: Web of Shadows (PSP)]
    "rkv",
    "rnd",
    "rof",
    "rpgmvo",
    "rrds",
    "rsd",
    "rsf",
    "rsm",
    "rsnd", //txth/reserved [Birushana: Ichijuu no Kaze (Switch)]
    "rsp",
    "rstm", //fake extension/header id for .rstm (in bigfiles)
    "rvws",
    "rwar",
    "rwav",
    "rws",
    "rwsd",
    "rwx",
    "rxx", //txth/reserved [Full Auto (X360)]

    "s14",
    "s3s", //txth/reserved [DT Racer (PS2)]
    "s3v", //Sound Voltex (AC)
    "sab",
    "sad",
    "saf",
    "sag",
    "sam", //txth/reserved [Lost Kingdoms 2 (GC)]
    "sap",
    "sb0",
    "sb1",
    "sb2",
    "sb3",
    "sb4",
    "sb5",
    "sb6",
    "sb7",
    "sbk",
    "sbin",
    "sbr",
    "sbv",
    "sig",
    "slb", //txth/reserved [Vingt-et-un Systems PS2 games (Last Escort, etc]
    "sm0",
    "sm1",
    "sm2",
    "sm3",
    "sm4",
    "sm5",
    "sm6",
    "sm7",
    "sc",
    "scd",
    "sch",
    "sd9",
    "sdp", //txth/reserved [Metal Gear Arcade (AC)]
    "sdf",
    "sdt",
    "se",
    "seb",
    "sed",
    "seg",
    "sem", //txth/reserved [Oretachi Game Center Zoku: Sonic Wings (PS2)]
    "sf0",
    "sfl",
    "sfs",
    "sfx",
    "sgb",
    "sgd",
    "sgt",
    "slb", //txth/reserved [THE Nekomura no Hitobito (PS2)]
    "sli",
    "smc",
    "smk",
    "smp",
    "smv",
    "snb",
    "snd",
    "snds",
    "sng",
    "sngw",
    "snr",
    "sns",
    "snu",
    "snz", //txth/reserved [Killzone HD (PS3)]
    "sod",
    "son",
    "spd",
    "spm",
    "sps",
    "spsd",
    "spw",
    "srsa",
    "ss2",
    "ssd", //txth/reserved [Zack & Wiki (Wii)]
    "ssm",
    "sspr",
    "ssp",
    "sss",
    "ster",
    "sth",
    "stm",
    "str",
    "stream",
    "strm",
    "sts",
    "stv", //txth/reserved [Socio Art Logic PS2 games (Zero no Tsukaima games, Cambrian QTS, Shirogane no Soleil, etc)]
    "sts_cp3",
    "stx",
    "svag",
    "svs",
    "svg",
    "swag",
    "swav",
    "swd",
    "switch", //txth/reserved (.m4a-x.switch) [Ikinari Maou (Switch)]
    "switch_audio",
    "sx",
    "sxd",
    "sxd2",
    "sxd3",
    "szd",
    "szd1",
    "szd3",

    "tad",
    "tgq",
    "tgv",
    "thp",
    "tmx",
    "tra",
    "trk",
    "trs", //txth/semi [Kamiwaza (PS2), Shinobido (PS2)]
    "tun",
    "txth",
    "txtp",

    "u0",
    "ue4opus",
    "ulw", //txth/raw [Burnout (GC)]
    "um3",
    "utk",
    "uv",

    "v0",
    //"v1", //dual channel with v0
    "va3",
    "vab",
    "vag",
    "vai",
    "vam", //txth/reserved [Rocket Power: Beach Bandits (PS2)]
    "vas",
    "vawx",
    "vb", //txth/reserved [Tantei Jinguji Saburo: Mikan no Rupo (PS1)]
    "vbk",
    "vbx", //txth/reserved [THE Taxi 2 (PS2)]
    "vca", //txth/reserved [Pac-Man World (PS1)]
    "vcb", //txth/reserved [Pac-Man World (PS1)]
    "vds",
    "vdm",
    "vgi", //txth/reserved [Time Crisis II (PS2)]
    "vgm", //txth/reserved [Maximo (PS2)]
    "vgs",
    "vgv",
    "vh",
    "vid",
    "vig",
    "vis",
    "vm4", //txth/reserved [Elder Gate (PS1)]
    "vms",
    "vmu", //txth/reserved [Red Faction (PS2)]
    "voi",
    "vp6",
    "vpk",
    "vs",
    "vsf",
    "vsv",
    "vxn",

    "w",
    "waa",
    "wac",
    "wad",
    "waf",
    "wam",
    "was",
    //"wav", //common
    "wavc",
    "wave",
    "wavebatch",
    "wavm",
    "wavx", //txth/reserved [LEGO Star Wars (Xbox)]
    "wax",
    "way",
    "wb",
    "wb2",
    "wbd",
    "wbk",
    "wd",
    "wem",
    "wiive", //txth/semi [Rubik World (Wii)]
    "wic", //txth/reserved [Road Rash (SAT)-videos]
    "wip", //txth/reserved [Colin McRae DiRT (PC)]
    "wlv", //txth/reserved [ToeJam & Earl III: Mission to Earth (DC)]
    "wmus", //fake extension (to be removed)
    "wp2",
    "wpd",
    "wsd",
    "wsi",
    "wst", //txth/reserved [3jigen Shoujo o Hogo Shimashita (PC)]
    "wua",
    "wv2",
    "wv6",
    "wvd", //txth/reserved [Donkey Kong Barrel Blast (Wii)]
    "wve",
    "wvs",
    "wvx",
    "wxd",

    "x",
    "x360audio", //fake extension for Unreal Engine 3 XMA (real extension unknown)
    "xa",
    "xa2",
    "xa30",
    "xai",
    "xag", //txth/reserved [Tamsoft's PS2 games]
    "xau",
    "xav",
    "xb", //txth/reserved [Scooby-Doo! Unmasked (Xbox)]
    "xen",
    "xma",
    "xma2",
    "xms",
    "xmu",
    "xmv",
    "xnb",
    "xsh",
    "xsf",
    "xse",
    "xsew",
    "xss",
    "xvag",
    "xvas",
    "xwav", //fake extension for .wav (renamed, to be removed)
    "xwb",
    "xmd",
    "xopus",
    "xps",
    "xwc",
    "xwm",
    "xwma",
    "xws",
    "xwv",

    "ydsp",
    "ymf",

    "zic",
    "zsd",
    "zsm",
    "zss",
    "zwdsp",
    "zwv",

    "vgmstream" /* fake extension, catch-all for FFmpeg/txth/etc */

    //, NULL //end mark
};
