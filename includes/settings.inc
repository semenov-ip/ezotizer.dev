<?php
function show_advertiser_settings(){
	global $base;
	global $var;

	$user = $base->exec('select name, family, phone, icq, curr_num
	from '.$var['base_tab_prefix'].'users
	where id="'.$var['user_id'].'"');

	if(count($user) == 1){
		$name = $user[0]['name'];
		$family = $user[0]['family'];
		$phone = $user[0]['phone'];
		$icq = $user[0]['icq'];
		$curr_num = $user[0]['curr_num'];
	}

	if($_POST['postadvertisersettings'] == 'post'){
		$name = htmlspecialchars(trim(post_str_val('name')));
		$family = htmlspecialchars(trim(post_str_val('family')));
		$phone = htmlspecialchars(trim(post_str_val('phone')));
		$icq = htmlspecialchars(trim(post_str_val('icq')));
		$curr_num = htmlspecialchars(trim(post_str_val('curr_num')));
		$psw = htmlspecialchars(trim(post_str_val('psw')));

		$newpsw = htmlspecialchars(trim(post_str_val('newpsw')));
		$newpsw2 = htmlspecialchars(trim(post_str_val('newpsw2')));

		if(!$name){
			$error = 'не указано имя.';
		}

		/*if(!$error){
			if(!$family){
				$error = 'не указана фамилия.';
			}
		}*/

		if(!$error){
			if($newpsw){
				if(strlen($newpsw) < 6){
					$error = 'не верный новый пароль. Длина пароля должна быть не менее 6 символов.';
				}

				if(!$error){
					if(!eregi("([a-z0-9]+)", $newpsw)){
						$error = 'не верный новый пароль. Пароль может содержать только цифры и буквы латиницы.';
					}
				}

				if(!$error){
					if($newpsw != $newpsw2){
						$error = 'не верный новый пароль, набранный еще раз.';
					}
				}

				if(!$error){
					$change_psw = true;
					$date = $var['datetime'];
					$new_user_hash = md5($var['user_email'].$newpsw.$date);
				}
			}
		}

		if(!$error){
			if(!$psw){
				$error = 'не указан текущий пароль.';
			}
		}

		if(!$error){
        	$tmp_user = $base->exec('select count(*)
        	from '.$var['base_tab_prefix'].'users
        	where id="'.$var['user_id'].'"
        	and psw="'.md5($psw).'"');

        	if($tmp_user[0][0] != 1){
        		$error = 'не верный текущий пароль.';
        	}
		}

		if(!$error){
        	$base->exec('update '.$var['base_tab_prefix'].'users
			set
			name = "'.$name.'",
			family = "'.$family.'",
			phone = "'.$phone.'",
			icq = "'.$icq.'",
			curr_num = "'.$curr_num.'"
			'.($change_psw == true ? ', psw="'.md5($newpsw).'", last_data_update="'.$date.'", hash="'.$new_user_hash.'"' : '').'
			where id="'.$var['user_id'].'"');

			if($change_psw == true){
                $_SESSION['user_hash'] = $new_user_hash;
				@setcookie('user_hash', $new_user_hash, time() + 2592000, '/');

            	$mess = 'Новый пароль: '.$newpsw."\r\n";

	       		$from = 'no-reply@'.$var['url'];

	       		$subject = '=?Windows-1251?B?'.trim(@imap_binary('Новый пароль')).'?=';
				$headers = 'Return-Path: '.$from.''."\r\n";
				$headers .= 'MIME-Version: 1.0'."\r\n";
				$headers .= 'Content-Transfer-Encoding: 8bit'."\r\n";
				$headers .= 'Content-Type: text/plain; charset="Windows-1251"'."\r\n";
				$headers .= 'X-Mailer: PHP v.'.phpversion()."\r\n";
				$headers .= 'From: =?Windows-1251?B?'.trim(imap_binary(addcslashes($var['title'], "\"!@\\!@(!@)"))).'?= <'.$from.'>'."\r\n";
				$headers .= 'Reply-to: '.$from."\r\n";

				@mail($var['user_email'], $subject, $mess, $headers);
			}

			$postok = 'Изменения сохранены.';
		}
	}

	$txt .= '<h1>Настройки профиля</h1>';

	if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>Ошибка:</b> '.$error.'</div>';
	}

	if($postok){
		$txt .= '<div class="postok"><img src="/i/ok.gif" width="16" height="16" align="absmiddle"> '.$postok.'</div>';
	}

	$txt .= '
	<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
	 <table class="form">
      <tr>
       <td width="25%">Имя*</td>
       <td><div style="width: 280px;"><input name="name" type="text" value="'.$name.'" class="inp"></div></td>
      </tr>
	';

	/*$txt .= '
      <tr>
       <td>Фамилия*</td>
       <td><div style="width: 280px;"><input name="family" type="text" value="'.$family.'" class="inp"></div></td>
      </tr>
      <tr>
       <td>Телефон</td>
       <td><div style="width: 180px;"><input name="phone" type="text" value="'.$phone.'" class="inp"></div></td>
      </tr>
	';*/

	$txt .= '
      <tr>
       <td>ICQ, Skype</td>
       <td><div style="width: 180px;"><input name="icq" type="text" value="'.$icq.'" class="inp"></div></td>
      </tr>
      <tr>
       <td>Новый пароль</td>
       <td>
        <div style="width: 230px;"><input name="newpsw" type="password" class="inp"></div>
       </td>
      </tr>
      <tr>
       <td>Новый пароль, еще раз</td>
       <td>
        <div style="width: 230px;"><input name="newpsw2" type="password" class="inp"></div>
       </td>
      </tr>
      <tr>
       <td>Текущий пароль*</td>
       <td><div style="width: 230px;"><input name="psw" type="password" class="inp"></div></td>
      </tr>
      <tr>
       <td></td>
       <td>
        <input name="postadvertisersettings" type="hidden" value="post"><input type="submit" value="Сохранить" class="btn">
       </td>
      </tr>
     </table>
    </form>
	';

	return $txt;
}

function show_webmaster_settings(){
	global $base;
	global $var;

	$user = $base->exec('select name, family, top10_login, notshow_top10_login, phone, icq, curr_num
	from '.$var['base_tab_prefix'].'users
	where id="'.$var['user_id'].'"');

	if(count($user) == 1){
		$name = $user[0]['name'];
		$family = $user[0]['family'];
		$top10_login = $user[0]['top10_login'];
		$notshow_top10_login = $user[0]['notshow_top10_login'];
		$phone = $user[0]['phone'];
		$icq = $user[0]['icq'];
		$curr_num = $user[0]['curr_num'];

		if($curr_num != ''){
			$old_curr_num = $curr_num;
			$not_edit_curr_num = true;
		}
	}

	if($_POST['postadvertisersettings'] == 'post'){
		$name = htmlspecialchars(trim(post_str_val('name')));
		$family = htmlspecialchars(trim(post_str_val('family')));
		$top10_login = htmlspecialchars(trim(post_str_val('top10_login')));
		$notshow_top10_login = post_int_val('notshow_top10_login');
		$phone = htmlspecialchars(trim(post_str_val('phone')));
		$icq = htmlspecialchars(trim(post_str_val('icq')));
		$curr_num = htmlspecialchars(trim(post_str_val('curr_num')));
		$psw = htmlspecialchars(trim(post_str_val('psw')));

		$newpsw = htmlspecialchars(trim(post_str_val('newpsw')));
		$newpsw2 = htmlspecialchars(trim(post_str_val('newpsw2')));

		if(!$name){
			$error = 'не указано имя.';
		}

		/*if(!$error){
			if(!$family){
				$error = 'не указана фамилия.';
			}
		}*/

		if(!$error){
			if($not_edit_curr_num && $curr_num != $old_curr_num){
				$curr_num = $old_curr_num;
				$error = 'в целях безопасности, вы не можете изменить номер счет Яндекс.Деньги.';
			}
		}

		if(!$error){
			if(!$curr_num){
				$error = 'не указан номер счет Яндекс.Деньги.';
			}
		}

		if(!$error){
			if(strlen($curr_num) > 17 && strlen($curr_num) < 12){
				$error = 'не верно указан номер счет Яндекс.Деньги.';
			}
		}

		if(!$error){
			if($newpsw){
				if(strlen($newpsw) < 6){
					$error = 'не верный новый пароль. Длина пароля должна быть не менее 6 символов.';
				}

				if(!$error){
					if(!eregi("([a-z0-9]+)", $newpsw)){
						$error = 'не верный новый пароль. Пароль может содержать только цифры и буквы латиницы.';
					}
				}

				if(!$error){
					if($newpsw != $newpsw2){
						$error = 'не верный новый пароль, набранный еще раз.';
					}
				}

				if(!$error){
					$change_psw = true;
					$date = $var['datetime'];
					$new_user_hash = md5($var['user_email'].$newpsw.$date);
				}
			}
		}

		if(!$error){
			if(!$psw){
				$error = 'не указан текущий пароль.';
			}
		}

		if(!$error){
        	$tmp_user = $base->exec('select count(*)
        	from '.$var['base_tab_prefix'].'users
        	where id="'.$var['user_id'].'"
        	and psw="'.md5($psw).'"');

        	if($tmp_user[0][0] != 1){
        		$error = 'не верный текущий пароль.';
        	}
		}

		if(!$error){
        	$base->exec('update '.$var['base_tab_prefix'].'users
			set
			name = "'.$name.'",
			family = "'.$family.'",
			top10_login = "'.$top10_login.'",
			notshow_top10_login = "'.$notshow_top10_login.'",
			phone = "'.$phone.'",
			icq = "'.$icq.'",
			curr_num = "'.$curr_num.'"
			'.($change_psw == true ? ', psw="'.md5($newpsw).'", last_data_update="'.$date.'", hash="'.$new_user_hash.'"' : '').'
			where id="'.$var['user_id'].'"');

			if($change_psw == true){
                $_SESSION['user_hash'] = $new_user_hash;
				@setcookie('user_hash', $new_user_hash, time() + 2592000, '/');

            	$mess = 'Новый пароль: '.$newpsw."\r\n";

	       		$from = 'no-reply@'.$var['url'];

	       		$subject = '=?Windows-1251?B?'.trim(@imap_binary('Новый пароль')).'?=';
				$headers = 'Return-Path: '.$from.''."\r\n";
				$headers .= 'MIME-Version: 1.0'."\r\n";
				$headers .= 'Content-Transfer-Encoding: 8bit'."\r\n";
				$headers .= 'Content-Type: text/plain; charset="Windows-1251"'."\r\n";
				$headers .= 'X-Mailer: PHP v.'.phpversion()."\r\n";
				$headers .= 'From: =?Windows-1251?B?'.trim(imap_binary(addcslashes($var['title'], "\"!@\\!@(!@)"))).'?= <'.$from.'>'."\r\n";
				$headers .= 'Reply-to: '.$from."\r\n";

				@mail($var['user_email'], $subject, $mess, $headers);
			}

			$postok = 'Изменения сохранены.';
		}
	}

	$txt .= '<h1>Настройки профиля</h1>';

	if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>Ошибка:</b> '.$error.'</div>';
	}

	if($postok){
		$txt .= '<div class="postok"><img src="/i/ok.gif" width="16" height="16" align="absmiddle"> '.$postok.'</div>';
	}

	$txt .= '
	<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
	 <table class="form">
      <tr>
       <td width="25%">Имя*</td>
       <td><div style="width: 280px;"><input name="name" type="text" value="'.$name.'" class="inp"></div></td>
      </tr>
	';

	/*$txt .= '
      <tr>
       <td>Фамилия*</td>
       <td><div style="width: 280px;"><input name="family" type="text" value="'.$family.'" class="inp"></div></td>
      </tr>
	';*/

	$txt .= '
      <tr>
       <td>Логин для рейтинга*</td>
       <td><div style="width: 280px;"><input name="top10_login" type="text" value="'.$top10_login.'" class="inp"></div></td>
      </tr>
      <tr>
       <td></td>
       <td><input name="notshow_top10_login" id="notshow_top10_login" type="checkbox" value="1"'.($notshow_top10_login == '1' ? ' checked' : '').'><label for="notshow_top10_login"> Не участвовать в рейтинге мастеров.</label></td>
      </tr>
	';

	/*$txt .= '
      <tr>
       <td>Телефон</td>
       <td><div style="width: 180px;"><input name="phone" type="text" value="'.$phone.'" class="inp"></div></td>
      </tr>
	';*/

	$txt .= '
      <tr>
       <td>ICQ, Skype</td>
       <td><div style="width: 180px;"><input name="icq" type="text" value="'.$icq.'" class="inp"></div></td>
      </tr>
      <tr>
       <td>Яндекс.Деньги*</td>
       <td><div style="width: 110px;"><input name="curr_num" type="text" value="'.($curr_num != '' ? $curr_num : '').'" maxlength="15" class="inp"></div></td>
      </tr>
      <tr>
       <td>Новый пароль</td>
       <td>
        <div style="width: 230px;"><input name="newpsw" type="password" class="inp"></div>
       </td>
      </tr>
      <tr>
       <td>Новый пароль, еще раз</td>
       <td>
        <div style="width: 230px;"><input name="newpsw2" type="password" class="inp"></div>
       </td>
      </tr>
      <tr>
       <td>Текущий пароль*</td>
       <td><div style="width: 230px;"><input name="psw" type="password" class="inp"></div></td>
      </tr>
      <tr>
       <td></td>
       <td>
        <input name="postadvertisersettings" type="hidden" value="post"><input type="submit" value="Сохранить" class="btn">
       </td>
      </tr>
     </table>
    </form>
	';

	return $txt;
}
?>