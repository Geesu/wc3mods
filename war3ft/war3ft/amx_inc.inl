public on_StatusIcon(id) {

	new szStatusItem[8]
	read_data(2, szStatusItem, 7)

	if ( equali(szStatusItem, "defuser") ){
		if ( read_data(1) )
			p_data_b[id][PB_DEFUSE] = true
		else
			p_data_b[id][PB_DEFUSE] = false
	}

	if ( equali(szStatusItem, "buyzone") ){
		if ( read_data(1) )
			p_data_b[id][PB_BUYZONE] = true
		else
			p_data_b[id][PB_BUYZONE] = false
	}

	return PLUGIN_CONTINUE
}