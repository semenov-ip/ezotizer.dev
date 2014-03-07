<?php
function show_partner_link(){
	global $base;
	global $var;

	$txt .= '
	<p>Ссылка для привлечения рефералов: <a href="/?pid='.$var['user_id'].'">http://'.$_SERVER['SERVER_NAME'].'/?pid='.$var['user_id'].'</a></p>';

	return $txt;
}

function show_partner_referals(){
	global $base;
	global $var;

	$txt .= '
	<table class="list">
	 <tr class="head">
	  <td>Дата регистрации</td>
	  <td>#UIN</td>
	  <td>Доход от реферала</td>
	 </tr>
	';

	$user = $base->exec('select id, dataadd
	from '.$var['base_tab_prefix'].'users
	where pid = "'.$var['user_id'].'"
	order by dataadd desc');

	if(count($user) > 0){
		foreach($user as $user){
			$count_click = 0.00;

			$stat = $base->exec('select sum(count_rur) as count_rur
			from '.$var['base_tab_prefix'].'partners_stat
			where user_id = "'.$user['id'].'"
			and partner_id = "'.$var['user_id'].'"');

			$count_click = ($stat[0]['count_rur'] ? $stat[0]['count_rur'] : '0.00');

			$txt .= '
			<tr'.($num?' class="even"':'').'>
		  	 <td>'.date2str($user['dataadd']).'</td>
		  	 <td>'.$user['id'].'</td>
		   	 <td>'.$count_click.'</td>
		 	</tr>
		 	';

		 	$total_count_click += $count_click;

		 	$num = ($num *= -1) + 1;
		}

	}else{
		$txt .= '
		<tr>
		 <td colspan="3" align="center">список пуст.</td>
		</tr>
		';
	}

	$txt .= '
	 <tr class="footer b">
	  <td colspan="2">Итого:</td>
	  <td>'.float_to_mysql($total_count_click).'</td>
	 </tr>
	</table>
	';

	return $txt;
}
?>