<?php
function show_invitations(){
	global $base;
	global $var;

	$action = get_str_val('action');

	$invite_cnt = $base->exec('select count(id) from '.$var['base_tab_prefix'].'invites
	where user_id = "'.$var['user_id'].'"
	and status = "0"');

	if($invite_cnt[0][0] < 5){
		for($i = 0; $i < (5 - $invite_cnt[0][0]); $i++){
			$code = substr(strtoupper(md5($var['user_id'].$var['datetime'].$i)), 0, 15);

			$base->exec('insert into '.$var['base_tab_prefix'].'invites
			(
			user_id,
			code,
			dataadd,
			status
			)values(
			"'.$var['user_id'].'",
            "'.$code.'",
            "'.$var['datetime'].'",
            "0"
			)');
		}
	}

	$invite = $base->exec('select code, status from '.$var['base_tab_prefix'].'invites
	where user_id = "'.$var['user_id'].'"
	and status = "0"
	order by dataadd asc
	limit 5');

	$txt .= '
	<table class="list">
	 <tr class="head">
	  <td>Номер приглашения</td>
	 </tr>
	';

	if(count($invite) != 0){

		foreach($invite as $invite){
			$txt .= '
			<tr>
			 <td>'.$invite['code'].'</td>
			</tr>
			';
		}

	}else{
		$txt .= '
		<tr>
		 <td align="center">список пуст.</td>
		</tr>
		';
	}

	$txt .= '
	<tr>
	 <td class="footer"></td>
	</tr>
	';

	$txt .= '</table>';

	return $txt;
}
?>