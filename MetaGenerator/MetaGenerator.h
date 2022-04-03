#pragma once
#include "clang-c/Index.h"

struct ClangMetadata
{
	std::string displayName;
	std::string entityName;
	CXCursorKind cursorKind;
	CX_CXXAccessSpecifier accessSpecifier;
	CXTypeKind typeKind;
	std::string typeName;
	bool isPtr = false;
	bool isPOD = false;
	CXTypeKind returnTypeKind;
	std::string returnTypeName;
	size_t arraySize = 0;
	std::vector<ClangMetadata*> Children;
	ClangMetadata* Parent = nullptr;
	std::string parentDisplayName;
	std::string parentEntityName;
	CXTypeKind parentTypeKind;
	size_t TypeSize = 0;
	size_t BitOffset = 0;
	size_t ByteOffset = 0;

	bool operator==(ClangMetadata& RHS) const
	{
		return displayName == RHS.displayName && entityName == RHS.entityName && typeName == RHS.typeName;
	}

	bool IsChildOf(ClangMetadata& PossibleParent) const
	{
		return parentDisplayName == PossibleParent.displayName && parentEntityName == PossibleParent.entityName && parentTypeKind == PossibleParent.typeKind;
	}

	bool IsMetaType() const
	{
		for (ClangMetadata* Child : Children)
		{
			if (Child->displayName == "META_ANNOTATE")
			{
				return true;
			}
		}
		return false;
	}
};

struct VisitorData
{
	std::ostream* LogInfo;
	bool bJustVisitedClassDecl = false;
	bool bWithinClassDecl = false;
	std::string ParentName = std::string();
	bool bWithinInvalidClass = false;
	std::vector<ClangMetadata> GeneratedClangMetaData;
};


struct ThreadData
{
	std::vector<std::filesystem::path> files;
	std::filesystem::path srcpath;
	std::filesystem::path pchpath;
	std::filesystem::path metapath;
	std::string result;
	long long millisTotal;
};
