Cookie = {
	isCookieOn: function(){
		return navigator.cookieEnabled;
	},

	getBrowserName: function(){
		return this.searchString(this.dataBrowser) || "An unknown browser";
	},

	searchString: function(data){
		for (var i=0;i<data.length;i++){
			var dataString = data[i].string;
			var dataProp = data[i].prop;
			if (dataString) {
				if (dataString.indexOf(data[i].subString) != -1)
					return data[i].identity;
			}
			else if (dataProp)
				return data[i].identity;
		}
	},

	dataBrowser: [
		{
			string: navigator.userAgent,
			subString: "Chrome",
			identity: "Chrome"
		},
		{ 	string: navigator.userAgent,
			subString: "OmniWeb",
			versionSearch: "OmniWeb/",
			identity: "OmniWeb"
		},
		{
			string: navigator.vendor,
			subString: "Apple",
			identity: "Safari",
			versionSearch: "Version"
		},
		{
			prop: window.opera,
			identity: "Opera",
			versionSearch: "Version"
		},
		{
			string: navigator.vendor,
			subString: "iCab",
			identity: "iCab"
		},
		{
			string: navigator.vendor,
			subString: "KDE",
			identity: "Konqueror"
		},
		{
			string: navigator.userAgent,
			subString: "Firefox",
			identity: "Firefox"
		},
		{
			string: navigator.vendor,
			subString: "Camino",
			identity: "Camino"
		},
		{		// for newer Netscapes (6+)
			string: navigator.userAgent,
			subString: "Netscape",
			identity: "Netscape"
		},
		{
			string: navigator.userAgent,
			subString: "MSIE",
			identity: "Explorer",
			versionSearch: "MSIE"
		},
		{
			string: navigator.userAgent,
			subString: "Gecko",
			identity: "Mozilla",
			versionSearch: "rv"
		},
		{ 		// for older Netscapes (4-)
			string: navigator.userAgent,
			subString: "Mozilla",
			identity: "Netscape",
			versionSearch: "Mozilla"
		}
	],

	getStringHrefToBrowserCookiesEnable: function(){
		switch(this.getBrowserName()){
			case 'Opera':
				return 'http://help.opera.com/Linux/10.50/ru/cookies.html';
			case 'Firefox':
				return 'http://support.mozilla.org/ru/kb/Управление cookies';
			case 'Chrome':
				return 'https://support.google.com/chrome/answer/95647?hl=ru';
			default:
				return 'https://www.google.ru/search?q=cookie';
		}
	}
};

// =============================================================================

System = {
	start: function(){
		if (!Cookie.isCookieOn()){
			jQuery('a').attr("href", "");
			jQuery('#workArea').html('<div id="mainErrorTextBlock" class="infoBlock errorBorder errorText">В браузере выключены cookie. Для работы сайта их необходимо включить. <a class="defaultHandler" href="' + Cookie.getStringHrefToBrowserCookiesEnable() + '">Инструкция</a></div>');
		}
		
		pageInit();
	}
};

