#include "Entity.h"
#include "Graphics/Renderer.h"

void Entity::AddPlayer(uint32_t id, sPlayerDescription desc)
{
	//m_players.insert_or_assign(id, desc);
}

void Entity::RemovePlayer(uint32_t id)
{
	//m_players.erase(id);
}

void Entity::SetPlayerDescription(uint32_t id, sPlayerDescription desc)
{
	m_players.insert_or_assign(id, desc);

	//m_players[id] = desc;
}

void Entity::Draw()
{
	for (auto& player : m_players)
	{
		//Renderer::AddCube(m_players[i].pos.x, m_players[i].pos.y, m_players[i].pos.z, i / 2.0f);
		//Renderer::AddLine3D(0, 0, 0, m_players[i].pos.x, m_players[i].pos.y, m_players[i].pos.z);
	
		Renderer::Get().AddCube(player.second.pos.x, player.second.pos.y, player.second.pos.z, player.first);
		Renderer::Get().AddLine3D(0, 0, 0, player.second.pos.x, player.second.pos.y, player.second.pos.z);
	}
	Renderer::Get().Draw();
}