Action()
{
	int i;
	int coordId = -1;
	char * token;
	
	web_set_max_html_param_len("2048");
	
	lr_start_transaction("GetCities");
	
	web_reg_save_param_regexp("ParamName=cities",
	                          "RegExp=([a-zA-Z\\s]+,\\w+)",
	                          "Ordinal=ALL",
	                          SEARCH_FILTERS,
	                          "Scope=Body",
	                          LAST);
	
	web_reg_save_param_regexp("ParamName=coords",
	                          "RegExp=(-?\\d+.\\d+\\,-?\\d+.\\d+)\\s*",
	                          "Ordinal=ALL",
	                          SEARCH_FILTERS,
	                          "Scope=Body",
	                          LAST);
	
	web_custom_request("GetCities",
	                   "URL={url}/xml/SOAP_server/ndfdXMLserver.php",
	                   "Method=POST",
	                   "Snapshot=t1.inf",
	                   "EncType=text/xml; charset=utf-8",
	                   "Body=<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
"<SOAP-ENV:Body>"
"<ns9309:LatLonListCityNames xmlns:ns9309=\"uri:DWMLgen\">"
"<displayLevel xsi:type=\"xsd:string\">1</displayLevel>"
"</ns9309:LatLonListCityNames>"
"</SOAP-ENV:Body>"
"</SOAP-ENV:Envelope>",
						LAST);
	
	for(i = 1; i <= atoi(lr_eval_string("{cities_count}")); i++)
    {
		if(strcmp(lr_eval_string("{city}"), lr_paramarr_idx("cities", i)) != 0)
		{
			coordId = i;
			break;
		}
    }
	
	if(coordId == -1)
	{
		lr_error_message("***** No such city ******");
        lr_end_transaction("GetCities", LR_FAIL);
        lr_exit(LR_EXIT_ITERATION_AND_CONTINUE, LR_FAIL);
	}
	else 
	{
		token = (char *)strtok(lr_paramarr_idx("coords", i), ",");
		lr_save_string(token, "lat");
		token = (char *)strtok(NULL, ",");
		lr_save_string(token, "longitude");
		
		lr_save_datetime ("%Y/%m/%d", DATE_NOW, "date");
		lr_output_message ("Last Day of Next Month is %s", lr_eval_string ("{date}"));
	}
	
    lr_end_transaction("GetCities", LR_PASS);
    
    lr_start_transaction("GetWeather");
    
	
	web_reg_save_param("maxTemp",
	                          "LB=Daily Maximum Temperature&lt;/name&gt;\n        &lt;value&gt;",
	                          "RB=&lt",
	                          LAST);
	web_reg_save_param("minTemp",
	                          "LB=Daily Minimum Temperature&lt;/name&gt;\n        &lt;value&gt;",
	                          "RB=&lt",
	                          LAST);
	web_reg_save_param("dewPoint",
	                          "LB=Dew Point Temperature&lt;/name&gt;\n        &lt;value&gt;",
	                          "RB=&lt",
	                          LAST);
	web_reg_save_param("windSpeed",
	                          "LB=Wind Speed&lt;/name&gt;\n        &lt;value&gt;",
	                          "RB=&lt",
	                          LAST);
	web_reg_save_param("rhs",
	                          "LB=Relative Humidity&lt;/name&gt;\n        &lt;value&gt;",
	                          "RB=&lt",
	                          LAST);
	 
    web_custom_request("GetWeather",
	                   "URL={url}/xml/SOAP_server/ndfdXMLserver.php",
	                   "Method=POST",
	                   "Snapshot=t2.inf",
	                   "EncType=text/xml; charset=utf-8",
	                   "Body=<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
"<SOAP-ENV:Body>"
"<ns3591:NDFDgen xmlns:ns3591=\"uri:DWMLgen\">"
"<latitude xsi:type=\"xsd:string\">{lat}</latitude>"
"<longitude xsi:type=\"xsd:string\">{longitude}</longitude>"
"<product xsi:type=\"xsd:string\">time-series</product>"
"<startTime xsi:type=\"xsd:string\">{date}T00:00:00</startTime>"
"<endTime xsi:type=\"xsd:string\">{date}T23:59:59</endTime>"
"<Unit xsi:type=\"xsd:string\">e</Unit>"
"<weatherParameters>"
"<maxt xsi:type=\"xsd:boolean\">1</maxt>"
"<mint xsi:type=\"xsd:boolean\">1</mint>"
"<dew xsi:type=\"xsd:boolean\">1</dew>"
"<wspd xsi:type=\"xsd:boolean\">1</wspd>"
"<rh xsi:type=\"xsd:boolean\">1</rh>"
"</weatherParameters>"
"</ns3591:NDFDgen>"
"</SOAP-ENV:Body>"
"</SOAP-ENV:Envelope>",
						LAST);
	
    lr_end_transaction("GetWeather", LR_PASS);
    
	return 0;
}
