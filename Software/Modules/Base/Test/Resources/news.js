News = {
	_save: function(_id, _newsTitle, _newsShortInfo, _newsBody, _callback){
		jQuery.post("/?class=news&subclass=edit&action=save",{
			id: _id,
			newsTitle: _newsTitle,
			newsShortInfo: _newsShortInfo,
			newsBody: _newsBody},
			function(jSonData){
				var result;
				eval('result =' + jSonData);
				_callback(result.res, result.msg, result.id);
			});
	},
	
	save: function(){
		News._save(jQuery('#newsId').text(), jQuery('#newsTitleEdit').val(), jQuery('#newsShortInfoEdit').val(), jQuery('#newsEditArea').val(), function(result, msg, id){
			if (result){
				jQuery('#newsEditTextBlock').removeClass('errorText errorBorder');
				jQuery('#newsEditTextBlock').addClass('infoText infoBorder');
				jQuery('#newsId').html(id);
			}
			else{
				jQuery('#newsEditTextBlock').removeClass('infoText infoBorder');
				jQuery('#newsEditTextBlock').addClass('errorText errorBorder');
			}

			jQuery('#newsEditTextBlock').text(msg);
		});
	},
			
	preview: function(){
		jQuery('#preview').removeAttr("hidden");
		jQuery('#preview').html(jQuery('#newsEditArea').val());
	}			
};
