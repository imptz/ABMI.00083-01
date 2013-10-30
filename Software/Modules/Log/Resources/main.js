System = {
	drawLog: function(res){
		var list = res.split('\n');
		var logList;
		list.forEach(function(str){
			if (str !== ''){
				logList += '<tr>';
				var items = str.split('\t');
				items.forEach(function(item){
						logList += ('<td>' + item + '</td>');
				});
				logList += '/<tr>';
			}
		});
		$('#logList').append(logList);
	},
	
	getLog: function(){
		$.get("getLog",{},
			function(res){
				System.drawLog(res);
			});

	},

	start: function(){
		System.getLog();
	}
};

