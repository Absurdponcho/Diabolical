#pragma once
#ifndef LIBCLANG_META
	#include <typeinfo>
#endif

class MetaBase 
{

};

// the definition will be generated for this.
#define IMPORT_META(x)

// We can annotate classes we want to generate meta data for. the MetaGenerator will ignore any classes without this annotation
#ifdef LIBCLANG_META
	#define METACLASS(...) class __attribute__((annotate(#__VA_ARGS__))) __attribute__((annotate("META_ANNOTATE")))

	#define PROPERTY(...) __attribute__((annotate(#__VA_ARGS__))) __attribute__((annotate("META_ANNOTATE")))

#else
	#define METACLASS(...) class
	#define PROPERTY(...) 
#endif
