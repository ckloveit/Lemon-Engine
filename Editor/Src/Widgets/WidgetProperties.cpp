#include "WidgetProperties.h"

#include "WidgetSceneHierachy.h"
#include "World/Components/StaticMeshComponent.h"
#include "World/Components/TransformComponent.h"
#include "WidgetHelpers.h"

WidgetProperties::WidgetProperties(Lemon::Engine* engine)
	:Widget(engine)
{
	m_WidgetProp.Title = WidgetGlobal::g_WidgetPropertiesTitle;
	m_WidgetProp.MinSize = glm::vec2(370.0f, -1.0f);

}

void WidgetProperties::Tick(float deltaTime)
{
	if(WidgetSceneHierachy::SelectEntity)
	{
		DrawEntity(WidgetSceneHierachy::SelectEntity);
	}
}

void WidgetProperties::DrawEntity(Lemon::Entity entity) const
{
	// Draw Name
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::string& entityName = entity.GetName();
		std::strncpy(buffer, entityName.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			entityName = std::string(buffer);
		}
	}
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");
	
	if (ImGui::BeginPopup("AddComponent"))
	{
		if (ImGui::MenuItem("StaticMeshComponent"))
		{
			if (!entity.HasComponent<Lemon::StaticMeshComponent>())
				entity.AddComponent<Lemon::StaticMeshComponent>();
			else
				LEMON_CORE_WARN("This entity already has the StaticMeshComponent!");
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	ImGui::PopItemWidth();

	if (entity.HasComponent<Lemon::TransformComponent>())
	{
		bool open = ImGui::TreeNodeEx((void*)typeid(Lemon::TransformComponent).hash_code(), treeNodeFlags, "Transform");

		if (open)
		{
			auto& tc = entity.GetComponent<Lemon::TransformComponent>();
			WidgetHelpers::DrawVec3Control("Translation", tc.Position);
			WidgetHelpers::DrawVec3Control("Rotation", tc.Rotation);
			WidgetHelpers::DrawVec3Control("Scale", tc.Scale, 1.0f);
			ImGui::TreePop();
		}
	}

	if(entity.HasComponent<Lemon::StaticMeshComponent>())
	{
		bool open = ImGui::TreeNodeEx((void*)typeid(Lemon::TransformComponent).hash_code(), treeNodeFlags, "StaticMeshComponent");
		if(open)
		{
			// 
			ImGui::TreePop();
		}
	}

}