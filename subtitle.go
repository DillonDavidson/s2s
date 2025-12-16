package main

type Subtitle struct {
	index uint32 // Subtitle order
	start string // Start time
	end   string // End time
	text  string // Full text
}

func NewSubtitle(index uint32, start string, end string, text string) Subtitle {
	return Subtitle{
		index: index,
		start: start,
		end:   end,
		text:  text,
	}
}
