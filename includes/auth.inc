<?php
function show_auth_form(){
	global $base;
	global $var;

	if($var['user_id']){
		@header('location: /');
		exit;
	}

	if($_POST['postauth'] == 'post'){
		$email = htmlspecialchars(trim($_POST['email']));
		$psw = htmlspecialchars(trim($_POST['psw']));

		if(!$email){
			$error = '�� ������ �����!';
		}

		if(!$error){
			if($psw == ''){
				$error = '�� ������ ������!';
			}
		}

		if(!$error){
			$user = $base->exec('select who, dataadd, last_data_update, action
			from '.$var['base_tab_prefix'].'users
			where email="'.$email.'" and psw="'.md5($psw).'"');

			if(count($user) == 0){
				$error = '����� ��� ������ ������� �� �����!';

        	}else{
				if($user[0]['action'] == 2){
                	$error = '��� ������� ������������!';
				}
var_dump($user);
				if(!$error){
					if($user[0]['last_data_update'] != '0000-00-00 00:00:00'){
						$user_hash = md5($email.$psw.$user[0]['last_data_update']);

					}else{
	        			$user_hash = md5($email.$psw.$user[0]['dataadd']);
	        		}

					$_SESSION['user_hash'] = $user_hash;
					@setcookie('user_hash', $user_hash, time()+2592000, '/');
					@setcookie('user_email', $email, time()+604800, '/');

					@header('location: '.($_POST['return_page'] ? $_POST['return_page'] : '/'.$user[0]['who'].'/'));exit;
				}
			}
		}
	}

	$txt .= '<div class="path">
 	<a href="/">�������</a> /
	<b>���� � �������</b>
	</div>';

	if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>������:</b> '.$error.'</div>';
	}

	$txt .= '
	<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
     <table class="form">
      <tr>
       <td width="25%">����� (e-mail)</td>
       <td><div style="width: 250px;"><input name="email" type="text" value="'.($email?$email:$_COOKIE['user_email']).'" class="inp"></div></td>
      </tr>
      <tr>
       <td>������</td>
       <td><div style="width: 250px;"><input name="psw" type="password" class="inp"></div></td>
      </tr>
      <tr>
       <td></td>
       <td><input name="return_page" type="hidden" value="'.$_REQUEST['return_page'].'"><input name="postauth" type="hidden" value="post"><input type="submit" value="�����" class="btn"></td>
      </tr>
      <tr>
       <td></td>
       <td class="lnk">
        &bull; <a href="/getNewPassword/">������ ������?</a><br/>
        &bull; <a href="/registration/">������������������</a>
       </td>
      </tr>
	 </table>
    </form>
	';

	return $txt;
}

function show_remind_pass_form(){
	global $base;
	global $var;

	if($var['user_id']){
		@header('location: /');
		exit;
	}

	if($_POST['postnewpass'] == 'post'){
		$email = htmlspecialchars(trim(post_str_val('email')));

		if(!$error){
			if($email == ''){
				$error = '�� ������ e-mail!';
			}
		}

		if(!$error){
			if(!strstr($email, "@") or !strstr($email, ".") or strlen($email) < 6 or eregi("([�-�]+)", $email)){
				$error = '�� ������� �� ������������ e-mail!';
			}
		}

		if(!$error){
			$user = $base->exec('select id from '.$var['base_tab_prefix'].'users
			where email="'.$email.'"');

			if(count($user) == 0){
				$error = '��������� ���� e-mail �� ���������������!';
			}
		}

		if(!$error){
  			$psw = rand(1000000, 9999999);
        	$hash = md5($email.$psw.$var['datetime']);

        	$base->exec('update '.$var['base_tab_prefix'].'users
			set
			psw="'.md5($psw).'",
			hash="'.$hash.'",
			last_data_update="'.$var['datetime'].'"
			where id="'.$user[0]['id'].'" and email="'.$email.'"');

			$mess = '��� ����� ������: '.$psw."\r\n";

       		$from = 'no-reply@'.$var['url'];

       		$subject = '=?Windows-1251?B?'.trim(@imap_binary('��� ����� ������')).'?=';
			$headers = 'Return-Path: '.$from.''."\r\n";
			$headers .= 'MIME-Version: 1.0'."\r\n";
			$headers .= 'Content-Transfer-Encoding: 8bit'."\r\n";
			$headers .= 'Content-Type: text/plain; charset="Windows-1251"'."\r\n";
			$headers .= 'X-Mailer: PHP v.'.phpversion()."\r\n";
			$headers .= 'From: =?Windows-1251?B?'.trim(imap_binary(addcslashes($var['title'], "\"!@\\!@(!@)"))).'?= <'.$from.'>'."\r\n";
			$headers .= 'Reply-to: '.$from."\r\n";

			if(@mail($email, $subject, $mess, $headers)){
				@setcookie('user_email', $email, time()+604800, '/');
				$email = '';
				$postok = '����� ������ ���������!';

			}else{
				$error = '�� ������� ��������� ������ � �������. ���������� �����.';
			}
		}
	}

    $txt .= '<div class="path">
 	<a href="/">�������</a> /
	<b>��������� ������ ������</b>
	</div>';

	if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>������:</b> '.$error.'</div>';
	}

	if($postok){
		$txt .= '<div class="postok"><img src="/i/ok.gif" width="16" height="16" align="absmiddle"> '.$postok.'</div>';
	}

	$txt .='
	<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
	 <table class="form">
      <tr>
       <td width="25%">��� e-mail</td>
       <td><div style="width: 250px;"><input name="email" type="text" value="'.$email.'" class="inp"></div></td>
      </tr>
      <tr>
       <td></td>
       <td><input name="postnewpass" type="hidden" value="post"><input type="submit" value="�������� ������" class="btn"></td>
      </tr>
      <tr>
       <td></td>
       <td class="lnk">
        &bull; <a href="/registration/">������������������</a><br/>
        &bull; <a href="/login/">����� � �������</a>
       </td>
      </tr>
	 </table>
	</form>
	';

	return $txt;
}
?>