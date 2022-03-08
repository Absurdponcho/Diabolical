#include "Scene.h"
#include "Maths/Maths.h"
#include "Logging/Logging.h"

DEntity DScene::SceneRoot;
flecs::query<Transform3D, Transform3D> DScene::TransformQuery;

void DScene::InitECSSystems()
{

	{	// Create Scene Root
		SceneRoot = DEntity::CreateEntity("Scene Root");
		SceneRoot.Set(Transform3D());
		SceneRoot.GetComponentMutable<Transform3D>()->Name = "SceneRoot";

		SceneRoot.Set(DScene());
	}

		DUtilityECS::GetECSWorld().system<Transform3D>()
			.kind(flecs::OnSet)
			.kind(flecs::OnAdd)
			.each([](DEntity Parent, Transform3D& Transform){Transform.ThisEntity = Parent;});


	{	// Create Transform Query
			TransformQuery = DUtilityECS::GetECSWorld().query_builder<Transform3D, Transform3D>()
				.arg(2).set(flecs::Cascade, flecs::ChildOf)
				.oper(flecs::Optional)
				.term(flecs::Disabled)
				.oper(flecs::Not)
				.build();
			
	}

	{	// Create Scene Updater
		struct SceneUpdater {};
		DUtilityECS::GetECSWorld().entity("Scene Updater").add<SceneUpdater>();
		DUtilityECS::GetECSWorld().system<SceneUpdater>("Scene Updater System")
			.kind(flecs::PreStore)
			.each([](SceneUpdater& Scene)
		{
			TransformQuery.each([&](DEntity Entity, Transform3D& ChildTransform, Transform3D& ParentTransformRef)
			{
				//for (size_t Index : Iterator)
				//{
					//Transform3D* ChildTransform = &ChildTransforms[Index]; 
					Transform3D* ParentTransform = &ParentTransformRef;
					//LOG(DString::Format("%i: %s -> %s", 0, ParentTransform ? *ParentTransform->Name : "",*ChildTransform.Name));
				//}
			});
		});
	}

	
}
