禁止网页文字选中:
	// IE Evitar seleccion de texto
	document.onselectstart=function(){
		if (event.srcElement.type != "text" && event.srcElement.type != "textarea" && event.srcElement.type != "password")
		return false
		else return true;
	};

	// FIREFOX Evitar seleccion de texto
	if (window.sidebar){
		document.onmousedown=function(e){
			var obj=e.target;
			if (obj.tagName.toUpperCase() == "INPUT" || obj.tagName.toUpperCase() == "TEXTAREA" || obj.tagName.toUpperCase() == "PASSWORD")
			return true;
			/*else if (obj.tagName=="BUTTON"){ return true; }*/
			else 
			return false;
		}
	}
	// End -->

