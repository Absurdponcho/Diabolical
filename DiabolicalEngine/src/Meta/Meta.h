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

private:
	static void ParseInheritanceTree();
};

class DMetaBase
{
public:
	virtual ~DMetaBase() {}
protected:

};

// ===================================================================================
// =============================== Class Meta ========================================
// ===================================================================================

class DClassMetaDataBase 
{
public:
	DClassMetaDataBase();
	int64_t Identifier = ClassMetaId;

	virtual const char* GetClassName() { return nullptr; };
	virtual size_t GetClassSize() { return 0; };
	void RegisterMetaProperty(class DPropertyMetaDataBase* NewProperty);
	virtual const DMetaBase* GetNullClass() { return nullptr; }
	virtual const DMetaBase* GetDefaultClass() { return nullptr; }

	virtual bool IsDerivedFrom(const DMetaBase* Object) { return false; }
private:
	void* RegisteredProperties = nullptr; // DVector<DPropertyMetaDataBase*>*
	void* Inherits = nullptr; // DVector<DClassMetaDataBase*>*
};

template<typename T>
class DClassMetaData : public DClassMetaDataBase
{
public:
	template<typename T2>
	DClassMetaData(T2* NewNullClass) 
		: NullClass(NewNullClass) 
	{
		DefaultClass = new T();
	}

	virtual size_t GetClassSize() override { return sizeof(T); }
	virtual const char* GetClassName() override { return typeid(T).name(); };
	virtual const DMetaBase* GetNullClass() override { return NullClass; }
	virtual const DMetaBase* GetDefaultClass() override { return DefaultClass; }
	virtual bool IsDerivedFrom(const DMetaBase* Object) override { 
		return dynamic_cast<const T*>(Object) != nullptr; 
	}

private:
	DMetaBase* NullClass = nullptr;
	DMetaBase* DefaultClass = nullptr;
};


#define REGISTER_CLASS(Class)																									\
private:																														\
	static_assert(std::is_convertible<Class *, DMetaBase*>::value, #Class " must inherit DMetaBase.");							\
	inline static Class* NullClass = nullptr;																					\
	__declspec(allocate("meta$u")) inline static DClassMetaData<Class> Meta_##Class = DClassMetaData<Class>(NullClass);			\
	DClassMetaData<Class>& StaticMutableMetaData() { return Meta_##Class; }														\
	 																															\
public:																															\
	const static DClassMetaData<Class>& StaticMetaData() { return Meta_##Class; }												\
	const DClassMetaData<Class>& GetMetaData() { return Meta_##Class; }															\
																																\

#define GENERATE_CLASS_META(Class)																								\
	public:																														\
	REGISTER_CLASS(Class)																										\
	inline const char* GetTypeName() { return typeid(Class).name(); }															\



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


#define PROPERTY_INTERNAL(Type, Name, DefaultValue)																																											\
	public:																																																					\
		typedef Type Name##_Property;																																														\
		Type Name DefaultValue;																																																\
	private:																																																				\
		__declspec(allocate("meta$u")) inline static DPropertyMetaData<Name##_Property>* Meta_##Name##_Property##;																											\
		static DPropertyMetaDataBase* MetaInitializerFunc_##Name##_Property##() 																																			\
		{ 																																																					\
			Meta_##Name##_Property## = new DPropertyMetaData<Name##_Property>(#Name, (size_t)&NullClass->Name);																												\
			static_cast<DClassMetaDataBase*>(&NullClass->StaticMutableMetaData())->RegisterMetaProperty(Meta_##Name##_Property##); 																							\
			return Meta_##Name##_Property##;																																												\
		}																																																					\
		__declspec(allocate("meta$u")) inline static DPropertyMetaInitializer MetaInitializer_##Name##_Property## = DPropertyMetaInitializer(MetaInitializerFunc_##Name##_Property##);										\
	public:																																																					\

#define PROPERTYDEF(Type, Name, DefaultValue) PROPERTY_INTERNAL(Type, Name, =##DefaultValue)
#define PROPERTY(Type, Name) PROPERTY_INTERNAL(Type, Name, ;)