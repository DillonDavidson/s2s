package main

const Arrow = "-->"

// FFmpeg commands
const (
	FFmpeg          = "ffmpeg"
	Input           = "-i"
	DisableVideo    = "-vn"
	VorbisEncodeOne = "-acodec"
	VorbisEncodeTwo = "libvorbis"
	Start           = "-ss"
	End             = "-to"
	CopyOne         = "-c"
	CopyTwo         = "copy"
	ImageOne        = "-vframes"
	ImageTwo        = "1"
	ScaleOne        = "-vf"
	ScaleTwo        = "scale=352:202"
)

// Quiet output
const (
	QuietOne   = "-hide_banner"
	QuietTwo   = "-loglevel"
	QuietThree = "error"
)

// File stuff
const (
	DefaultOutputFile = "audio.ogg"
	Media             = ".media"
	Ogg               = ".ogg"
	Webp              = ".webp"
	Tsv               = ".tsv"
	Rm                = "rm"
)

// Config stuff
const (
	Order      = "order"
	Config     = ".config"
	S2S        = "s2s"
	ConfigToml = "config.toml"
)

type AnkiField int

const (
	Expression AnkiField = iota
	SequenceNumber
	Audio
	Snapshot
	Tags
	IDK
)
