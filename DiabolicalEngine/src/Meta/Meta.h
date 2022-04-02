#pragma once

// We can annotate classes we want to generate meta data for. the MetaGenerator will ignore any classes without this annotation
#ifdef __clang__
	#define METACLASS class __attribute__((annotate("meta"))) 
#else
	#define METACLASS class
#endif