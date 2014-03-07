<?php
function show_tickets(){
	global $base;
	global $var;

	$txt .= '
	<div class="section_menu">
	 <a href="./?action=add"><img src="/i/add.gif" width="16" height="16" align="absmiddle">Создать тикет</a>
	</div>
	';

	$txt .= '
	<table class="list">
	 <tr class="head">
	  <td>Дата</td>
	  <td>Заголовок</td>
	 </tr>
	';

	$ticket = $base->exec('select id, name, text, last_dataadd, status
	from '.$var['base_tab_prefix'].'tickets
	where user_id = "'.$var['user_id'].'"
	and upid = "0"
	order by last_dataadd desc');

	if(count($ticket) != 0){
		foreach($ticket as $ticket){
			$num = 0;

			$txt .= '
			<tr'.($num?' class="even"':'').'>
			 <td>'.date2str($ticket['last_dataadd']).'</td>
			 <td'.($ticket['status'] == '1' ? ' class="b"' : '').'><a href="./?action=edit&ticket_id='.$ticket['id'].'">'.$ticket['name'].'</a></td>
			</tr>
			';
		}

	}else{
		$txt .= '
		<tr>
		 <td colspan="2" align="center">список пуст.</td>
		</tr>
		';
	}

	$txt .= '
	 <tr>
	  <td colspan="2" class="footer">&nbsp;</td>
	 </tr>
	</table>
	';

	return $txt;
}

function show_ticket_add(){
	global $base;
	global $var;

	if($_POST['postnewticket'] == 'post'){
		$name = stripslashes(htmlspecialchars(trim(post_str_val('name'))));
		$text = stripslashes(htmlspecialchars(trim(post_str_val('text'))));

		if(!$name){
			$error = 'не указан заголовок!';
		}

		if(!$error){
			if(!$text){
				$error = 'не указан текст!';
			}
		}

		if(!$error){
			$base->exec('insert into '.$var['base_tab_prefix'].'tickets
			(
			user_id,
			author_name,
			name,
			text,
			dataadd,
			last_dataadd,
			status
			)values(
			"'.$var['user_id'].'",
			"'.$var['user_email'].'",
			"'.$name.'",
			"'.$text.'",
			"'.$var['datetime'].'",
			"'.$var['datetime'].'",
			"2"
			)');

			$id = $base->new_id();
			if($id){
				@header('location: ./');
				exit;

			}else{
            	$error = 'не удалось создать тикет. Попробуйте позже.';
			}
		}
	}

	$txt .= '<h1>Создание тикета</h1>';

	$txt .= '<p></p>';

	if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>Ошибка:</b> '.$error.'</div>';
	}

	$txt .= '
	<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
	 <table class="form">
	  <tr>
       <td>Заголовок</td>
       <td><div style="width: 350px;"><input name="name" type="text" value="'.$name.'" class="inp"></div></td>
      </tr>
      <tr>
       <td>Текст</td>
       <td><div style="width: 350px;"><textarea name="text">'.$text.'</textarea></div></td>
      </tr>
      <tr>
       <td></td>
       <td>
        <input name="postnewticket" type="hidden" value="post">
        <input type="submit" value="Сохранить" class="btn">
       </td>
      </tr>
	 </table>
	</form>
	';

	return $txt;
}

function show_ticket(){
	global $base;
	global $var;

	$ticket_id = get_int_val('ticket_id');

	$ticket = $base->exec('select author_name, name, text, dataadd
	from '.$var['base_tab_prefix'].'tickets
	where user_id = "'.$var['user_id'].'"
	and id = "'.$ticket_id.'"');

	if(count($ticket) == 0){
		@header('location: ./');
		exit;
	}

	$base->exec('update '.$var['base_tab_prefix'].'tickets
	set status = "2"
	where user_id = "'.$var['user_id'].'"
	and id = "'.$ticket_id.'"');

	$ticket[0]['text'] = str_replace("\n", "<br/>", $ticket[0]['text']);

	$txt .= '<h1>'.$ticket[0]['name'].'</h1>';

	$txt .= '<p></p>';

	$txt .= '
	<table class="list">
	 <tr class="head">
	  <td>Дата</td>
	  <td>Автор</td>
	  <td>Текст</td>
	 </tr>
	';

	$txt .= '
	<tr>
	 <td>'.date2str($ticket[0]['dataadd']).'</td>
	 <td>'.$ticket[0]['author_name'].'</td>
	 <td>'.$ticket[0]['text'].'</td>
	</tr>
	';

	$ticket = $base->exec('select author_name, text, dataadd
	from '.$var['base_tab_prefix'].'tickets
	where user_id = "'.$var['user_id'].'"
	and upid = "'.$ticket_id.'"
	order by dataadd asc');

	if(count($ticket) != 0){
	 	$num = 1;
		foreach($ticket as $ticket){
			$ticket['text'] = str_replace("\n", "<br/>", $ticket['text']);

			$txt .= '
			<tr'.($num?' class="even"':'').'>
			 <td>'.date2str($ticket['dataadd']).'</td>
			 <td>'.$ticket['author_name'].'</td>
	 		 <td>'.$ticket['text'].'</td>
			</tr>
			';

			$num = ($num *= -1) + 1;
		}
	}

	$txt .= '
	 <tr>
	  <td colspan="3" class="footer">&nbsp;</td>
	 </tr>
	</table>
	';

	$txt .= '<p></p>';

	if($_POST['postanswerticket'] == 'post'){
		$text = stripslashes(htmlspecialchars(trim(post_str_val('text'))));

		if(!$text){
			$error = 'не указан текст!';
		}

		if(!$error){
			$base->exec('insert into '.$var['base_tab_prefix'].'tickets
			(
			upid,
			user_id,
			author_name,
			text,
			dataadd
			)values(
			"'.$ticket_id.'",
			"'.$var['user_id'].'",
			"'.$var['user_email'].'",
			"'.$text.'",
			"'.$var['datetime'].'"
			)');

			$id = $base->new_id();
			if($id){
				$base->exec('update '.$var['base_tab_prefix'].'tickets
				set
				last_dataadd = "'.$var['datetime'].'",
				admin_status = "1"
				where id="'.$ticket_id.'"');

				@header('location: ./?action=edit&ticket_id='.$ticket_id.'');
				exit;

			}else{
            	$error = 'не удалось сохранить ответ. Попробуйте позже.';
			}
		}
	}

	$txt .= '<h1>Добавление быстрого ответа</h1>';

	if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>Ошибка:</b> '.$error.'</div>';
	}

	$txt .= '
	<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
	 <table class="form">
      <tr>
       <td>Текст</td>
       <td><div style="width: 350px;"><textarea name="text">'.$text.'</textarea></div></td>
      </tr>
      <tr>
       <td></td>
       <td>
        <input name="postanswerticket" type="hidden" value="post">
        <input type="submit" value="Сохранить" class="btn">
       </td>
      </tr>
	 </table>
	</form>
	';

	return $txt;
}
?>