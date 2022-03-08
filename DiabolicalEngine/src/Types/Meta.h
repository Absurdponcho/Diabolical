#pragma once


#pragma section("meta$a",read,write)
#pragma section("meta$u",read,write)
#pragma section("meta$z",read,write)

#include <typeinfo>

#define ClassMetaId 0xdeadbeefdeadc0de
#define PropertyMetaId ClassMetaId+1
#define PropertyMetaInitializerId ClassMetaId+2

class DMeta
{
public:
	static void ParseMetaData();


};



// ===================================================================================
// =============================== Class Meta ========================================
// ===================================================================================

class DClassMetaDataBase 
{
public:
	int64_t Identifier = ClassMetaId;

	virtual const char* GetClassName() { return nullptr; };
	virtual size_t GetClassSize() { return 0; };
};

template<typename T>
class DClassMetaData : public DClassMetaDataBase
{
public:
	DClassMetaData() {}
	virtual size_t GetClassSize() override { return sizeof(T); }
	virtual const char* GetClassName() override { return typeid(T).name(); };
};

struct EmptyInitializer {};

#define REGISTER_CLASS(Class)																									\
private:																														\
	__declspec(allocate("meta$u")) inline static DClassMetaData<Class> Meta_##Class = DClassMetaData<Class>();					\
public:																															\
	const static DClassMetaData<Class>& StaticMetaData() { return Meta_##Class; }												\
	const DClassMetaData<Class>& GetMetaData() { return Meta_##Class; }															\
																																\

#define GENERATE_CLASS_META(Class)																								\
	public:																														\
	REGISTER_CLASS(Class)																										\
	inline const char* GetTypeName() { return typeid(Class).name(); }															\
	Class(EmptyInitializer e){}																									\
	inline static Class* NullClass = nullptr;																					\


// ===================================================================================
// =============================== Property Meta =====================================
// ===================================================================================

class DPropertyMetaInitializer
{
public:
	DPropertyMetaInitializer(class DPropertyMetaDataBase* (*NewFuncPtr)())
		: FuncPtr(NewFuncPtr) {}

	const int64_t Identifier = PropertyMetaInitializerId;
	class DPropertyMetaDataBase* (*FuncPtr)();
};

class DPropertyMetaDataBase
{
public:
	const int64_t Identifier = PropertyMetaId;
	virtual const char* GetPropertyName() { return nullptr; }
	virtual const char* GetTypeName() { return nullptr; }
	virtual size_t GetPropertySize() { return 0; }
	virtual size_t GetPropertyOffset() { return 0; }

};

template<typename T>
class DPropertyMetaData : public DPropertyMetaDataBase
{
public:
	DPropertyMetaData(const char* NewName, size_t NewOffset)
		: Name(NewName), Offset(NewOffset) {}

	virtual size_t GetPropertySize() override { return sizeof(T); }
	virtual const char* GetTypeName() override { return typeid(T).name(); }
	virtual const char* GetPropertyName() override { return Name; }
	virtual size_t GetPropertyOffset() override { return Offset; }

	template<typename O>
	void SetValue(O& Owner, T Value)
	{
		T* OwnerValuePtr = (((size_t)&Owner) + Offset);
		*OwnerValuePtr = Value;
	}

private:
	size_t Offset;
	const char* Name = nullptr;
};
					
#define PROPERTY(Type, Name)																													\
	Type Name;																																	\
__declspec(allocate("meta$u")) inline static DPropertyMetaData<Type>* Meta_##Type##_##Name;														\
static DPropertyMetaDataBase* MetaInitializerFunc_##Type##_##Name() { Meta_##Type##_##Name = new DPropertyMetaData<Type>(#Name, (size_t)&NullClass->Name); return Meta_##Type##_##Name; }	\
__declspec(allocate("meta$u")) inline static DPropertyMetaInitializer MetaInitializer_##Type##_##Name = DPropertyMetaInitializer(MetaInitializerFunc_##Type##_##Name);												\
																																				\


