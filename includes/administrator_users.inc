<?php
function show_administrator_users(){
	global $base;
	global $var;

	$txt .= '
	<table class="list">
	 <tr class="head">
	  <td>Дата</td>
	  <td>E-mail</td>
	  <td>Имя</td>
	  <td>Фамилия</td>
	  <td>Телефон</td>
	  <td>ICQ</td>
	  <td>Баланс</td>
	 </tr>
	';

	$user = $base->exec('select id, email, name, family, phone, icq, count_rur, dataadd, hash
	from '.$var['base_tab_prefix'].'users
	order by dataadd desc');

	foreach($user as $user){
    	if(strlen($user['email']) > 10){
		$user_email = substr($user['email'], 0, 10).'...';

		}else{
			$user_email = $user['email'];
		}

		$txt .= '
    	<tr'.($num ? ' class="even"' : '').'>
		 <td>'.date2str($user['dataadd']).'</td>
		 <td><a href="/?change_user_hash='.$user['hash'].'" title="'.$user['email'].'">'.$user_email.'</a></td>
		 <td>'.$user['name'].'</td>
		 <td>'.$user['family'].'</td>
		 <td>'.$user['phone'].'</td>
		 <td>'.$user['icq'].'</td>
		 <td>'.$user['count_rur'].'</td>
		</tr>
    	';

    	$num = ($num *= -1) + 1;
	}

	$txt .= '
	<tr class="footer">
	 <td colspan="7">&nbsp;</td>
	 </td>
	</tr>
	';

	$txt .= '
	</table>
	';

	return $txt;
}
?>