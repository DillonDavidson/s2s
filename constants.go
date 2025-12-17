package main

const Arrow = "-->"

// FFmpeg commands
const (
	FFmpeg       = "ffmpeg "
	Input        = " -i "
	DisableVideo = " -vn "
	VorbisEncode = " -acodec libvorbis "
	Start        = " -ss "
	End          = " -to "
	Copy         = " -c copy "
	Image        = " -vframes 1 "
	Scale        = " -vf \"scale=352:202\" "
)

// Quiet output
const (
	Quiet        = " -hide_banner -loglevel error "
	ShellBeQuiet = " > /dev/null 2>&1 "
)

// File stuff
const (
	DefaultOutputFile = "audio.ogg"
	Media             = ".media"
	Ogg               = ".ogg"
	Webp              = ".webp"
	Tsv               = ".tsv"
	Rm                = "rm "
)
