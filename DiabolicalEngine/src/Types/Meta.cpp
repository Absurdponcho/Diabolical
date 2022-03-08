#include "Meta.h"
#include "Logging/Logging.h"
#include "Types/DString.h"
#include "Check.h"
#include "DVector.h"


__declspec(allocate("meta$a")) int64_t MetaSectionStart = 0;
__declspec(allocate("meta$z")) int64_t MetaSectionEnd = 0;


DVector<DClassMetaDataBase*> ClassMetaData;
DVector<DPropertyMetaDataBase*> PropertyMetaData;

void DMeta::ParseMetaData()
{
	uint8_t* CurrentAddr = (uint8_t*)&MetaSectionStart;
	uint8_t* EndAddr = (uint8_t*)&MetaSectionEnd;

	DString LogString;
	while (CurrentAddr != EndAddr)
	{
		DClassMetaDataBase* PossibleClass = (DClassMetaDataBase*)CurrentAddr;

		if (PossibleClass->Identifier == ClassMetaId)
		{
			ClassMetaData.PushBack(PossibleClass);
			LogString.Append(DString::Format("\n    %s (%i bytes)", PossibleClass->GetClassName(), PossibleClass->GetClassSize()));
		}
			
		DPropertyMetaInitializer* PossiblePropertyInitializer = (DPropertyMetaInitializer*)CurrentAddr;

		if (PossiblePropertyInitializer->Identifier == PropertyMetaInitializerId)
		{
			DPropertyMetaDataBase* Property = PossiblePropertyInitializer->FuncPtr();
			PropertyMetaData.PushBack(Property);
			LogString.Append(DString::Format("\n        %i-> %s %s", Property->GetPropertyOffset(), Property->GetTypeName(), Property->GetPropertyName()));
		}
		CurrentAddr++;
	}
	LogString.Append("\n");
	LOG(LogString);
	
}
