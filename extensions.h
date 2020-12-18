static const char* extension_list[] = {
    //"", /* vgmstream can play extensionless files too, but plugins must accept them manually */

    "04sw",
    "208",
    "2dx9",
    "2pfs",
    "3do",
    "4", //for Game.com audio
    "8", //txth/reserved [Gungage (PS1)]
    "800",
    "9tav",

    //"aac", //common
    "aa3", //FFmpeg/not parsed (ATRAC3/ATRAC3PLUS/MP3/LPCM/WMA)
    "aax",
    "abc", //txth/reserved [Find My Own Way (PS2) tech demo]
    "abk",
    //"ac3", //common, FFmpeg/not parsed (AC3)
    "acb",
    "acm",
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
    "aif-Loop",
    "aifc", //common?
    "aifcl", //fake extension for .aif???
    //"aiff", //common
    "aiffl", //fake extension for .aif???
    "aix",
    "akb",
    "al",
    "al2",
    "ams", //txth/reserved [Super Dragon Ball Z (PS2) ELF names]
    "amts", //fake extension/header id for .stm (renamed? to be removed?)
    "ao",
    "ap",
    "apc",
    "as4",
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
    "atx",
    "aud",
    "aus",
    "awb",
    "awc",

    "b1s",
    "baf",
    "baka",
    "bank",
    "bar",
    "bcstm",
    "bcwav",
    "bd3",
    "bdsp",
    "bfstm",
    "bfwav",
    "bfwavnsmbu", //fake extension for New Super Smash Bros U (renamed to fix bug)
    "bg00",
    "bgm",
    "bgw",
    "bh2pcm",
    "bik",
    "bika", //fake extension for .bik (to be removed)
    "bik2",
    //"bin", //common
    "bk2",
    "blk",
    "bmdx",
    "bms",
    "bnk",
    "bnm",
    "bns",
    "bnsf",
    "bo2",
    "brstm",
    "brstmspm",
    "btsnd",
    "bvg",
    "bwav",

    "caf",
    "capdsp",
    "cbd2",
    "ccc",
    "cd",
    "cfn", //fake extension for CAF (renamed, to be removed?)
    "chk",
    "ckb",
    "ckd",
    "cks",
    "cnk",
    "cpk",
    "cps",
    "csa", //txth/reserved [LEGO Racers 2 (PS2)]
    "csmp",
    "cvs",
    "cxs",

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
    "diva",
    "dmsg",
    "ds2", //txth/reserved [Star Wars Bounty Hunter (GC)]
    "dsb",
    "dsf",
    "dsp",
    "dspw",
    "dtk",
    "dvi",
    "dxh",

    "e4x",
    "eam",
    "eas",
    "eda", //txth/reserved [Project Eden (PS2)]
    "emff", //fake extension for .mul (to be removed)
    "enm",
    "eno",
    "ens",
    "exa",
    "ezw",

    "fag",
    "fda",
    "ffw",
    "filp",
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
    "genh",
    "gin",
    "gms",
    "gsb",
    "gsf",
    "gtd",
    "gwm",

    "h4m",
    "hab",
    "hca",
    "hdr",
    "hgc1",
    "his",
    "hps",
    "hsf",
    "hwx", //txth/reserved [Star Wars Episode III (Xbox)]
    "hx2",
    "hx3",
    "hxc",
    "hxd",
    "hxg",
    "hxx",
    "hwas",

    "iab",
    "iadp",
    "idmsf",
    "idsp",
    "idvi", //fake extension/header id for .pcm (renamed, to be removed)
    "idwav",
    "idx",
    "idxma",
    "ikm",
    "ild",
    "ilv", //txth/reserved [Star Wars Episode III (PS2)]
    "ima",
    "imc",
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
    "kovs", //fake extension/header id for .kvs
    "kns",
    "kraw",
    "ktsl2asbin",
    "ktss", //fake extension/header id for .kns
    "kvs",

    "l",
    "l00", //txth/reserved [Disney's Dinosaur (PS2)]
    "laac", //fake extension for .aac (tri-Ace)
    "laif", //fake extension for .aif (various)
    "laiff", //fake extension for .aiff
    "laifc", //fake extension for .aifc
    "lac3", //fake extension for .ac3, FFmpeg/not parsed
    "lasf", //fake extension for .asf (various)
    "lbin", //fake extension for .bin (various)
    "leg",
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
    "lopus", //fake extension for .opus
    "lp",
    "lpcm",
    "lpk",
    "lps",
    "lrmb",
    "lse",
    "lsf",
    "lstm", //fake extension for .stm
    "lwav", //fake extension for .wav
    "lwma", //fake extension for .wma, FFmpeg/not parsed

    "mab",
    "mad",
    "map",
    "matx",
    "mc3",
    "mca",
    "mcadpcm",
    "mcg",
    "mds",
    "mdsp",
    "med",
    "mi4",
    "mib",
    "mic",
    "mihb",
    "mnstr",
    "mogg",
    //"m4a", //common
    //"m4v", //common
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
    "msf",
    "mss",
    "msv",
    "msvp",
    "mta2",
    "mtaf",
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
    "ndp",
    "ngca",
    "nlsd",
    "nop",
    "nps",
    "npsf", //fake extension/header id for .nps (in bigfiles)
    "nsopus",
    "nub",
    "nub2",
    "nus3audio",
    "nus3bank",
    "nwa",
    "nwav",
    "nxa",

    //"ogg", //common
    "ogl",
    "oma", //FFmpeg/not parsed (ATRAC3/ATRAC3PLUS/MP3/LPCM/WMA)
    "omu",
    //"opus", //common
    "opusx",
    "otm",
    "oto", //txth/reserved [Vampire Savior (SAT)]
    "ovb",

    "p04", //txth/reserved [Psychic Force 2012 (DC), Skies of Arcadia (DC)]
    "p16", //txth/reserved [Astal (SAT)]
    "p1d", //txth/reserved [Farming Simulator 18 (3DS)]
    "p2a", //txth/reserved [Thunderhawk Operation Phoenix (PS2)]
    "p2bt",
    "p3d",
    "past",
    "pcm",
    "pdt",
    "pk",
    "pnb",
    "pona",
    "pos",
    "ps2stm", //fake extension for .stm (renamed? to be removed?)
    "psf",
    "psh", //fake extension for .vsv (to be removed)
    "psnd",
    "psw", //fake extension for .wam (renamed, to be removed)

    "r",
    "rac", //txth/reserved [Manhunt (Xbox)]
    "rad",
    "rak",
    "ras",
    "raw",
    "rda", //FFmpeg/reserved [Rhythm Destruction (PC)]
    "rkv",
    "rnd",
    "rof",
    "rpgmvo",
    "rrds",
    "rsd",
    "rsf",
    "rsm",
    "rsp",
    "rstm", //fake extension/header id for .rstm (in bigfiles)
    "rvws",
    "rwar",
    "rwav",
    "rws",
    "rwsd",
    "rwx",
    "rxw",
    "rxx", //txth/reserved [Full Auto (X360)]

    "s14",
    "s3v", //txth/reserved [Sound Voltex 5 (AC)]
    "sab",
    "sad",
    "saf",
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
    "sbr",
    "sbv",
    "sm0",
    "sm1",
    "sm2",
    "sm3",
    "sm4",
    "sm5",
    "sm6",
    "sm7",
    "sbin",
    "sc",
    "scd",
    "sch",
    "sd9",
    "sdf",
    "sdt",
    "seb",
    "sed",
    "seg",
    "sf0",
    "sfl",
    "sfs",
    "sfx",
    "sgb",
    "sgd",
    "sgx",
    "sl3",
    "slb", //txth/reserved [THE Nekomura no Hitobito (PS2)]
    "sli",
    "smc",
    "smk",
    "smp",
    "smpl", //fake extension/header id for .v0/v1 (renamed, to be removed)
    "smv",
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
    "ss2",
    "ssd", //txth/reserved [Zack & Wiki (Wii)]
    "ssm",
    "sss",
    "ster",
    "sth",
    "stm",
    "stma", //fake extension/header id for .stm
    "str",
    "stream",
    "strm",
    "sts",
    "stx",
    "svag",
    "svs",
    "svg",
    "swag",
    "swav",
    "swd",
    "switch_audio",
    "sx",
    "sxd",
    "sxd2",
    "sxd3",

    "tec",
    "tgq",
    "thp",
    "tk5",
    "tmx",
    "tra",
    "tun",
    "txth",
    "txtp",
    "tydsp",

    "ue4opus",
    "ulw",
    "um3",
    "utk",
    "uv",

    "v0",
    //"v1", //dual channel with v0
    "va3",
    "vag",
    "vai",
    "vam", //txth/reserved [Rocket Power: Beach Bandits (PS2)]
    "vas",
    "vawx",
    "vb",
    "vbk",
    "vbx", //txth/reserved [THE Taxi 2 (PS2)]
    "vds",
    "vdm",
    "vgm", //txth/reserved [Maximo (PS2)]
    "vgs",
    "vgv",
    "vid",
    "vig",
    "vis",
    "vms",
    "vmu", //txth/reserved [Red Faction (PS2)]
    "voi",
    "vp6",
    "vpk",
    "vs",
    "vsf",
    "vsv",
    "vxn",

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
    "way",
    "wb",
    "wb2",
    "wbd",
    "wd",
    "wem",
    "wii",
    "wip", //txth/reserved [Colin McRae DiRT (PC)]
    "wlv", //txth/reserved [ToeJam & Earl III: Mission to Earth (DC)]
    "wma", //common
    "wmus",
    "wp2",
    "wpd",
    "wsd",
    "wsi",
    "wua",
    "wv2",
    "wv6",
    "wve",
    "wvs",
    "wvx",

    "x",
    "xa",
    "xa2",
    "xa30",
    "xag",
    "xau",
    "xav",
    "xb", //txth/reserved [Scooby-Doo! Unmasked (Xbox)]
    "xen",
    "xma",
    "xma2",
    "xmu",
    "xnb",
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
