public bool:IsClientAlive(client)  // From rollthedice_commands.sp
{
  return bool:!GetEntData(client,OffSet_LifeState,1);
}

public SetClientHealth(index, health)
{
	SetEntProp(index,Prop_Send,"m_iHealth",health,4);
}
