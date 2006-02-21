public log_function(info[]){
	new configsDir[64];
	get_configsdir(configsDir, 63);

	new fileInfo[64], string[128];
	format(fileInfo,63,"addons/amxmodx/logs/war3x.log", configsDir);

	format(string,127,"%s",info);

	write_file(fileInfo, string, -1);
	
	return PLUGIN_CONTINUE;
}
