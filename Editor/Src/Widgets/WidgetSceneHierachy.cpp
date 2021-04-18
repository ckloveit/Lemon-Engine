#include "WidgetSceneHierachy.h"


#include "World/Entity.h"
#include "World/World.h"

Lemon::Entity WidgetSceneHierachy::SelectEntity = {};

WidgetSceneHierachy::WidgetSceneHierachy(Lemon::Engine* engine)
	:Widget(engine)
{
	m_WidgetProp.Title = WidgetGlobal::g_WidgetSceneHierachyTitle;
	m_WidgetProp.WindowFlags |= ImGuiWindowFlags_HorizontalScrollbar;

}

void WidgetSceneHierachy::Tick(float deltaTime)
{
	DrawHierachyEntityTree();

}

void WidgetSceneHierachy::DrawHierachyEntityTree()
{
	std::vector<Lemon::Entity> AllEntitys = m_Engine->GetSystem<Lemon::World>()->GetAllEntities();
	for(int i = 0;i < AllEntitys.size(); i++)
	{
		if(AllEntitys[i].IsGizmo())
			continue;// don't draw gizmo entity in hierachy
		// Draw Tree Item
		ImGuiTreeNodeFlags flags = ((SelectEntity == AllEntitys[i]) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;//
		flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
		const bool bOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)AllEntitys[i], flags, AllEntitys[i].GetName().c_str());
		if (ImGui::IsItemClicked())
		{
			SelectEntity = AllEntitys[i];
		}
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}
		if (bOpened)
		{
			const ImGuiTreeNodeFlags temFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			const bool opened = ImGui::TreeNodeEx((void*)9817239, temFlags, AllEntitys[i].GetName().c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
		
		if (entityDeleted)
		{
			m_Engine->GetSystem<Lemon::World>()->DestroyEntity(AllEntitys[i]);
			if (SelectEntity == AllEntitys[i])
				SelectEntity = {};
		}
	}

	//No select
	if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
		SelectEntity = {};	
	}
	// Right-click on blank space
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		if (ImGui::MenuItem("Create Empty Entity"))
			m_Engine->GetSystem<Lemon::World>()->CreateEntity("Empty Entity");
		ImGui::EndPopup();
	}
}