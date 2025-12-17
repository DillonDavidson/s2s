package main

type Subtitle struct {
	index int    // Subtitle order
	start string // Start time
	end   string // End time
	text  string // Full text
}

func NewSubtitle(index int, start string, end string, text string) Subtitle {
	return Subtitle{
		index: index,
		start: start,
		end:   end,
		text:  text,
	}
}
