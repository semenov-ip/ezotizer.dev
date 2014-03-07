<?php
function show_administrator_sending(){
	global $base;
	global $var;

	if($_POST['postsending'] == 'post'){
		$title = stripslashes(trim(post_str_val('title')));
		$text = stripslashes(trim(post_str_val('text')));

		if(!$title){
			$error = 'не указана тема.';
		}

		if(!$error){
			if(!$text){
				$error = 'не указан текст.';
			}
		}

		if(!$error){
			$user = $base->exec('select email
			from '.$var['base_tab_prefix'].'users
			where last_data_auth != "0000-00-00 00:00:00"
			order by dataadd desc');

			$user_cnt = count($user);

			$from = 'no-reply@'.$var['url'];
			$subject = '=?Windows-1251?B?'.trim(@imap_binary($title)).'?=';

			foreach($user as $user){
				$headers = 'Return-Path: '.$from.''."\r\n";
				$headers .= 'MIME-Version: 1.0'."\r\n";
				$headers .= 'Content-Transfer-Encoding: 8bit'."\r\n";
				$headers .= 'Content-Type: text/plain; charset="Windows-1251"'."\r\n";
				$headers .= 'X-Mailer: PHP v.'.phpversion()."\r\n";
				$headers .= 'From: =?Windows-1251?B?'.trim(imap_binary(addcslashes($var['title'], "\"!@\\!@(!@)"))).'?= <'.$from.'>'."\r\n";
				$headers .= 'Reply-to: '.$from."\r\n";

				@mail($user['email'], $subject, $text, $headers);
			}

			$postok = 'Рассылка завершена. Отправлено '.$user_cnt.' '.num2word_end($user_cnt, '', 'письмо', 'письма', 'писем').'.';
		}
	}

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
       <td>Тема</td>
       <td><div style="width: 350px;"><input name="title" type="text" value="'.$title.'" class="inp"></div></td>
      </tr>
      <tr>
       <td>Текст</td>
       <td>
        <div style="width: 350px;"><textarea name="text">'.$text.'</textarea></div>
       </td>
      </tr>
      <tr>
       <td></td>
       <td>
        <input name="postsending" type="hidden" value="post">
        <input type="submit" value="Отправить" class="btn">
       </td>
      </tr>
	 </table>
	</form>
	';

	return $txt;
}
?>