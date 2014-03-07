<?php
function show_reg(){
	global $base;
	global $var;

	if($var['user_id']){
		@header('location: /');
		exit;
	}

	if($_POST['postnewuser'] == 'post'){
    	$email = htmlspecialchars(trim(post_str_val('email')));
    	$name = stripslashes(trim(post_str_val('name')));
    	$family = htmlspecialchars(stripslashes(trim(post_str_val('family'))));
    	$phone = htmlspecialchars(stripslashes(trim(post_str_val('phone'))));
    	$icq = htmlspecialchars(stripslashes(trim(post_str_val('icq'))));
    	$who = htmlspecialchars(stripslashes(trim(post_str_val('who'))));

		if(!$error){
			if($email == ''){
				$error = 'не указан e-mail.';
			}
		}

		if(!$error){
			if(!strstr($email, "@") or !strstr($email, ".") or strlen($email) < 6 or eregi("([а-я]+)", $email)){
				$error = 'указан не существующий e-mail.';
			}
		}

		if(!$error){
			$users = $base->exec('select id from '.$var['base_tab_prefix'].'users
			where email="'.$email.'"');

			if(count($users) > 0){
				$error = 'указанный e-mail уже зарегистрирован!';
			}
		}

		if(!$error){
			if($name == ''){
				$error = 'не указано имя.';
			}
		}

		if(!$error){
        	$psw = rand(1000000, 9999999);
        	$hash = md5($email.$psw.$var['datetime']);

      $base->exec('
        	insert into '.$var['base_tab_prefix'].'users(
			pid,
			email,
			psw,
			name,
			family,
			phone,
			icq,
			who,
			count_rur,
			dataadd,
			administrator,
			moderator,
			last_data_auth,
			last_data_update,
			hash,
			action
        	)values(
			"'.($_COOKIE['pid'] ? $_COOKIE['pid'] : 0).'",
			"'.$email.'",
			"'.md5($psw).'",
			"'.$name.'",
			"'.$family.'",
			"'.$phone.'",
			"'.$icq.'",
			"'.$who.'",
			"0.00",
			"'.$var['datetime'].'",
			"0",
			"0",
			"'.$var['datetime'].'",
			"'.$var['datetime'].'",
			"'.$hash.'",
			"1"
        	)');

      $id = $base->new_id();
			if($id){
				$mess = 'Поздравляем, вы успешно зарегистрированы.'."\r\n\r\n";
				$mess .= 'Логин (e-mail): '.$email."\r\n";
				$mess .= 'Пароль: '.$psw."\r\n";

        $from = 'no-reply@'.$var['url'];

        $subject = '=?Windows-1251?B?'.trim(imap_binary('Регистрация')).'?=';
				$headers = 'Return-Path: '.$from.''."\r\n";
				$headers .= 'MIME-Version: 1.0'."\r\n";
				$headers .= 'Content-Transfer-Encoding: 8bit'."\r\n";
				$headers .= 'Content-Type: text/plain; charset="Windows-1251"'."\r\n";
				$headers .= 'X-Mailer: PHP v.'.phpversion()."\r\n";
				$headers .= 'From: =?Windows-1251?B?'.trim(imap_binary(addcslashes($var['title'], "\"!@\\!@(!@)"))).'?= <'.$from.'>'."\r\n";
				$headers .= 'Reply-to: '.$from."\r\n";

				@setcookie('user_email', $email, time()+604800, '/');

				if(@mail($email, $subject, $mess, $headers)){
					$postok = 'Поздравляем, вы успешно зарегистрированы.
					Ваши логин и пароль отправлены на указанный e-mail.';
					$email = $name = $family = $phone = $icq = '';

				}else{
          $postok = 'Вы зарегистрированы. Чтобы получить пароль для входа в систему,
						пройдите по ссылке &laquo;Получить новый пароль&raquo;.';
					$email = $name = $family = $phone = $icq = '';
				}

			}else{
				$error = 'регистрация временно недоступна, попробуйте позже.';
			}
		}
	}

	$txt .= '<div class="path">
 	<a href="/">Главная</a> /
	<b>Регистрация в системе</b>
	</div>';

	if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>Ошибка:</b> '.$error.'</div>';
	}

	if($postok){
		$txt .= '<div class="postok"><img src="/i/ok.gif" width="16" height="16" align="absmiddle"> '.$postok.'</div>';
	}

	$txt .='
	<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
	 <table class="form">
      <tr>
       <td width="25%">Ваш e-mail*</td>
       <td><div style="width: 250px;"><input name="email" type="text" value="'.$email.'" class="inp"></div></td>
      </tr>
      <tr>
       <td>Имя*</td>
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
       <td>Начать работу как</td>
       <td>
        <table>
	';

	/*$txt .= '
         <tr>
          <td><input name="who" id="advertiser" type="radio" value="advertiser"'.($who == 'advertiser' ? ' checked' : (!$who ? 'checked' : '')).'> <label for="advertiser">Рекламодатель</label></td>
         </tr>
	';*/

	$txt .= '
         <tr>
          <td><input name="who" id="webmaster" type="radio" value="webmaster"'.($who == 'webmaster' ? ' checked' : (!$who ? 'checked' : '')).'> <label for="webmaster">Вебмастер</label></td>
         </tr>
        </table>
       </td>
      </tr>
      <tr>
       <td></td>
       <td><input type="checkbox" value="1" checked disabled><label>Я ознакомился с <a href="/agree/" target="_blank">правилами участия</a> и согласен их соблюдать.</label></td>
      </tr>
      <tr>
       <td></td>
       <td><input name="postnewuser" type="hidden" value="post"><input type="submit" value="Зарегистрироваться" class="btn"></td>
      </tr>
      <tr>
       <td></td>
       <td class="lnk">
        &bull; <a href="/login/">Войти в систему</a><br/>
        &bull; <a href="/getNewPassword/">Получить новый пароль</a>
       </td>
      </tr>
	 </table>
	</form>
	';

	return $txt;
}
?>