User = {
	testName: function(value){
		return /^[a-z0-9а-я_-]{3,25}$/i.test(value) ? true : false;
	},
	
	testPassword: function(value){
		return /^[a-z0-9а-я_-]{3,25}$/i.test(value) ? true : false;
	},
	
	testEmail: function(value){
		return /^[a-z0-9а-я_-]{1,25}@[a-zа-я0-9_-]{1,25}\.([a-zа-я0-9_-]{1,25}\.)?[a-zа-я0-9_-]{2,6}$/i.test(value) ? true : false;
	},

	RULE_NAME: 'Имя должно быть не короче 3 и не длиннее 25 символов. Может содержать только большие и маленькие буквы латинского и русского алфавита цифры от 0 до 9 и символы \'-\' и \'_\'', 
	RULE_PASSWORD: 'Пароль должнен быть не короче 3 и не длиннее 25 символов. Может содержать только большие и маленькие буквы латинского и русского алфавита цифры от 0 до 9 и символы \'-\' и \'_\'',
	RULE_EMAIL: '',
	
	_login: function(_nikname, _password, _callback){
		jQuery.post("/?class=user&subclass=login&action=login",{
			nikname: _nikname,
			password: _password},
			function(jSonData){
				var result;
				eval('result =' + jSonData);
				_callback(result.res, result.msg);
			});
	},
	
	login: function(){
		User._login(jQuery('#loginFormNikname').val(), jQuery('#loginFormPassword').val(), function(result, msg){
			if (result){
				jQuery('#loginTextBlock').removeClass('errorText errorBorder');
				jQuery('#loginTextBlock').addClass('infoText infoBorder');
				jQuery('#loginFormNikname').hide();
				jQuery('#loginFormPassword').hide();
				jQuery('#loginFormButton').hide();
				jQuery('#loginFormRefs').hide();
				jQuery('#headerBarNavigatorLoginLi').before('<li><a href="/?class=user&subclass=profile&action=form">Профиль</a></li>');
				jQuery('#headerBarNavigatorLoginButton').attr('href', '/?class=user&subclass=logout');
				jQuery('#headerBarNavigatorLoginButton').text('Выход');
			}
			else{
				jQuery('#loginTextBlock').removeClass('infoText infoBorder');
				jQuery('#loginTextBlock').addClass('errorText errorBorder');
			}

			jQuery('#loginText').text(msg);
		});
	},
	
	_registration: function(_nikname, _password, _secondPassword, _email, _callback){
		var testRegistrationData = function(_nikname, _password, _secondPassword, _email){
			if (!User.testName(_nikname)){
				throw 'nikname';
			}

			if (!User.testPassword(_password)){
				throw 'password';
			}

			if (!User.testPassword(_secondPassword)){
				throw 'secondPassword';
			}

			if (_password !== _secondPassword){
				throw 'password != secondPassword';
			}

			if (!User.testEmail(_email)){
				throw 'email';
			}
		};

		try{
			testRegistrationData(_nikname, _password, _secondPassword, _email);
			jQuery.post('/index.php',{
				class: 'user',
				subclass: 'registration',
				action: 'registration',
				nikname:_nikname,
				password:_password, 
				email:_email},
				function(jSonData){
					var result;
					eval('result =' + jSonData);
					_callback(result.res, result.type, result.msg);
				});
		}catch(err){
			throw err;
		}
	},
	
	registration: function(){
		var error = function($text, $id){
			jQuery('#userRegTextBlock').removeClass('infoText infoBorder');
			jQuery('#userRegTextBlock').addClass('errorText errorBorder');
			jQuery('#userRegText').text($text);
			jQuery($id).css('border', '1px solid #ff0000');
			jQuery($id).focus();
		};

		try{
			jQuery('#userRegFormNikname').css('border', '1px solid #3b5998');
			jQuery('#userRegFormPassword').css('border', '1px solid #3b5998');
			jQuery('#userRegFormPasswordConf').css('border', '1px solid #3b5998');
			jQuery('#userRegFormEmail').css('border', '1px solid #3b5998');
			
			User._registration(jQuery('#userRegFormNikname').val(), jQuery('#userRegFormPassword').val(), jQuery('#userRegFormPasswordConf').val(), jQuery('#userRegFormEmail').val(), function(result, type, msg){
				if (result){
					jQuery('#userRegFormNikname').css('visibility', 'hidden');
					jQuery('#userRegFormPassword').css('visibility', 'hidden');
					jQuery('#userRegFormPasswordConf').css('visibility', 'hidden');
					jQuery('#userRegFormEmail').css('visibility', 'hidden');
					jQuery('#userRegFormButton').css('visibility', 'hidden');
					jQuery('#userRegTextBlock').removeClass('errorText errorBorder');
					jQuery('#userRegTextBlock').addClass('infoText infoBorder');
				}
				else{
					jQuery('#userRegTextBlock').removeClass('infoText infoBorder');
					jQuery('#userRegTextBlock').addClass('errorText errorBorder');
					switch(type){
						case 'nameExists':
							jQuery('#userRegFormNikname').css('border', '1px solid #ff0000');
							jQuery('#userRegFormNikname').focus();
							break;
						case 'emailExists':
							jQuery('#userRegFormEmail').css('border', '1px solid #ff0000');
							jQuery('#userRegFormEmail').focus();
							break;
					}
				}

				jQuery('#userRegText').text(msg);		
			});
		}catch(err){
			if (err === 'nikname'){
				error('Ошибка. Ник не корректен. ' + User.RULE_NAME, '#userRegFormNikname');
				return false;
			}

			if (err === 'password'){
				error('Ошибка. Пароль не корректен. ' + User.RULE_PASSWORD, '#userRegFormPassword');
				return false;
			}

			if (err === 'secondPassword'){
				error('Ошибка. Повторный пароль не корректен. ' + User.RULE_PASSWORD, '#userRegFormPasswordConf');
				return false;
			}

			if (err === 'password != secondPassword'){
				error('Ошибка. Введенные пароли не совпадают. ' + User.RULE_PASSWORD, '#userRegFormPassword');
				jQuery('#userRegFormPasswordConf').css('border', '1px solid #ff0000');
				return false;
			}

			if (err === 'email'){
				error('Ошибка. Адрес электронной почты не корректен. ' + User.RULE_EMAIL, '#userRegFormEmail');
				return false;
			}
		}
	},
	
	_restore: function(_nikname, _callback){
		jQuery.post("/users/restore/restore",{
			nikname: _nikname},
			function(jSonData){
				var result;
				eval('result =' + jSonData);
				_callback(result.res, result.msg);
			});
	},
	
	restore: function(){
		User._restore(jQuery('#restoreFormNikname').val(), function(result, msg){
			if (result){
				jQuery('#restoreTextBlock').removeClass('errorText errorBorder');
				jQuery('#restoreTextBlock').addClass('infoText infoBorder');
				jQuery('#restoreFormNikname').hide();
				jQuery('#restoreFormPassword').hide();
				jQuery('#restoreFormButton').hide();
				jQuery('#restoreFormRefs').hide();
			}
			else{
				jQuery('#restoreTextBlock').removeClass('infoText infoBorder');
				jQuery('#restoreTextBlock').addClass('errorText errorBorder');
			}

			jQuery('#restoreText').text(msg);
		});
	},

	_profile: function(isDeleteUser){
		jQuery('#profileTextBlock').removeClass('errorText errorBorder');
		jQuery('#profileTextBlock').addClass('infoText infoBorder');
		jQuery('#profileText').text('Профиль пользователя');
		
		if (User.profile_validateForm())
			jQuery('#profileFormPassword').css('border', '1px solid #3b5998');
			jQuery('#profileFormPasswordConf').css('border', '1px solid #3b5998');
			jQuery('#profileFormEmail').css('border', '1px solid #3b5998');
			jQuery('#profileFormSurname').css('border', '1px solid #3b5998');
			jQuery('#profileFormName').css('border', '1px solid #3b5998');
			jQuery('#profileFormPatrinymic').css('border', '1px solid #3b5998');
			jQuery('#profileFormComment').css('border', '1px solid #3b5998');
			jQuery.post('/?class=user&subclass=profile&action=change',{
				password:jQuery('#profileFormPassword').val(),
				email:jQuery('#profileFormEmail').val(),
				surname:jQuery('#profileFormSurname').val(),
				name:jQuery('#profileFormName').val(),
				patronymic:jQuery('#profileFormPatronymic').val(),
				comment:jQuery('#profileFormComment').val(),
				deleteUser:isDeleteUser},
				function(jSonData){
					var result;
					eval('result =' + jSonData);
					
					if (result.res){
						jQuery('#profileTextBlock').removeClass('errorText errorBorder');
						jQuery('#profileTextBlock').addClass('infoText infoBorder');

						if (isDeleteUser)
							window.location.href = '/';
					}
					else{
						jQuery('#profileTextBlock').removeClass('infoText infoBorder');
						jQuery('#profileTextBlock').addClass('errorText errorBorder');
						switch(result.type){
							case 'emailExists':
								jQuery('#profileFormEmail').css('border', '1px solid #ff0000');
								jQuery('#profileFormEmail').focus();
								break;
						}
					}

					jQuery('#profileText').text(result.msg);
				});

		return false;
	},

	profile: function(){
		var checked = jQuery('#profileFormDeleteUser').is(':checked');

		if (checked){
			$("#deleteUserConfirmationDialog").dialog({
			  resizable: false,
			  height:140,
			  width:200,
			  modal: true,
			  buttons: {
				"Удалить": function() {
					$(this).dialog("close");
					User._profile(true);
				},
				"Отменить": function() {
				  $(this).dialog("close");
				}
			  }
			});
		}else{
			if (User.profile_validateForm()){
				User._profile(false);
			}
		}
	},

	profile_error: function($text, $id){
		jQuery('#profileTextBlock').removeClass('infoText infoBorder');
		jQuery('#profileTextBlock').addClass('errorBorder errorText');
		jQuery('#profileText').text($text);
		jQuery($id).css('border', '1px solid #ff0000');
		jQuery($id).focus();
	},

	profile_validateForm: function(){
		var password = jQuery('#profileFormPassword').val();
		var passwordConf = jQuery('#profileFormPasswordConf').val();
		var email = jQuery('#profileFormEmail').val();
		var surname = jQuery('#profileFormSurname').val();
		var name = jQuery('#profileFormName').val();
		var patronymic = jQuery('#profileFormPatronymic').val();
		var comment = jQuery('#profileFormComment').val();

		jQuery('#profileFormPassword').css('border', '1px solid #3b5998');
		jQuery('#profileFormPasswordConf').css('border', '1px solid #3b5998');
		jQuery('#profileFormEmail').css('border', '1px solid #3b5998');
		jQuery('#profileFormSurname').css('border', '1px solid #3b5998');
		jQuery('#profileFormName').css('border', '1px solid #3b5998');
		jQuery('#profileFormPatrinymic').css('border', '1px solid #3b5998');
		jQuery('#profileFormComment').css('border', '1px solid #3b5998');

		if ((password !== "") && (password !== undefined) && !User.testPassword(password)){
			User.profile_error('Ошибка. Пароль не корректен. ' + User.RULE_PASSWORD, '#profileFormPassword');
			return false;
		}

		if ((passwordConf !== "") && (passwordConf !== undefined) && !User.testPassword(passwordConf)){
			User.profile_error('Ошибка. Повторный пароль не корректен. ' + User.RULE_PASSWORD, '#profileFormPasswordConf');
			return false;
		}

		if (password !== passwordConf){
			User.profile_error('Ошибка. Введенные пароли не совпадают. ' + User.RULE_PASSWORD, '#profileFormPassword');
			jQuery('#profileFormPasswordConf').css('border', '1px solid #ff0000');
			return false;
		}

		if ((email !== "") && (email !== undefined) && !User.testEmail(email)){
			User.profile_error('Ошибка. Адрес электронной почты не корректен. ' + User.RULE_EMAIL, '#profileFormEmail');
			return false;
		}

		if ((surname !== "") && (surname !== undefined) && !User.testName(surname)){
			User.profile_error('Ошибка. Фамилия не корректна. ' + User.RULE_NAME, '#profileFormSurname');
			return false;
		}

		if ((name !== "") && (name !== undefined) && !User.testName(name)){
			User.profile_error('Ошибка. Имя не корректно. ' + User.RULE_NAME, '#profileFormName');
			return false;
		}

		if ((patronymic !== "") && (patronymic !== undefined) && !User.testName(patronymic)){
			User.profile_error('Ошибка. Отчесткво не корректно. ' + User.RULE_NAME, '#profileFormPatronymic');
			return false;
		}

		return true;
	}
};

