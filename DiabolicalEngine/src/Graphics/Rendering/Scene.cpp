#include "Scene.h"
#include "Maths/Maths.h"
#include "Logging/Logging.h"

DEntity DScene::SceneRoot;
flecs::query<Transform3D, Transform3D> DScene::TransformQuery;

void DScene::InitECSSystems()
{
		DUtilityECS::GetECSWorld().system<Transform3D>()
			.kind(flecs::OnSet)
			.kind(flecs::OnAdd)
			.each([](DEntity Parent, Transform3D& Transform){Transform.ThisEntity = Parent;});


	//{	// Create Transform Query
	//	TransformQuery = DUtilityECS::GetECSWorld().query_builder<Transform3D, Transform3D>()
	//		.arg(2).set(flecs::Cascade, flecs::ChildOf)
	//		.oper(flecs::Optional)
	//		.term(flecs::Disabled)
	//		.oper(flecs::Not)
	//		.build();
	//}

	//{	// Create Scene Updater
	//	struct SceneUpdater {};
	//	DUtilityECS::GetECSWorld().entity("Scene Updater").add<SceneUpdater>();
	//	DUtilityECS::GetECSWorld().system<SceneUpdater>("Scene Updater System")
	//		.kind(flecs::PreStore)
	//		.each([](SceneUpdater& Scene)
	//	{
	//		TransformQuery.iter([&](flecs::iter& Iterator, Transform3D* ChildTransform, Transform3D* ParentTransform)
	//		{
	//			for (size_t Index : Iterator)
	//			{
	//				LOG(DString::Format("%i: %s -> %s", Index, ParentTransform ? *ParentTransform->Name : "", ChildTransform ? *ChildTransform->Name : ""));
	//			}
	//		});
	//	});
	//}

	{	// Create Scene Root
		SceneRoot = DUtilityECS::GetECSWorld().entity("Scene")
			.add<Transform3D>()
			.add<DScene>();

		SceneRoot.get_mut<Transform3D>()->Name = "Scene Root";
	}
	
}
