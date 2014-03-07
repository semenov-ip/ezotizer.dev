<?php
function show_administrator_payout(){
	global $base;
	global $var;

	$status = get_str_val('status');

	if($_GET['status'] == ''){
		@header('location: ./?action=payout&status=0');
		exit;
	}

	if(is_array($_POST['check'])){
		$button = stripslashes(post_str_val('button'));

		if($button == 'Экспорт в CSV'){
			$check_arr = $_POST['check'];

			foreach($check_arr as $order_id){
             	if($ids_list == ''){
             		$ids_list = $order_id;

             	}else{
             		$ids_list .= ','.$order_id;
             	}
			}

			@header("Content-Type: application/csv");
		 	@header("Content-Disposition: attachment; filename=\"payout.csv\"");

			$history = $base->exec('select id, user_id, amount, dataadd
			from '.$var['base_tab_prefix'].'count_history
			where
			id in ('.$ids_list.')
			and trans_type = "0"
			and status = "0"
			order by dataadd desc');

			if(count($history) > 0){
				foreach($history as $history){
					$user = $base->exec('select email, curr_num
					from '.$var['base_tab_prefix'].'users
					where id="'.$history['user_id'].'"');

					$txt .= $user[0]['curr_num'].';';
					$txt .= $history['amount'].';';
					$txt .= $var['pay_title'].' - выплата по заказу - '.$user[0]['email'].';';
					$txt .= ''.$history['id'].''."\n";
				}
			}

			echo $txt;
			exit;

		}elseif(stripslashes($button) == 'Перенести в "Обработанные"'){
			$check_arr = $_POST['check'];

			foreach($check_arr as $order_id){
            	$base->exec('update '.$var['base_tab_prefix'].'count_history
				set status="1"
				where id="'.$order_id.'"');
			}

			@header('location: ./?action=payout');
			exit;

		}elseif($button == 'Отклонить'){
			$check_arr = $_POST['check'];

			foreach($check_arr as $order_id){
            	$base->exec('update '.$var['base_tab_prefix'].'count_history
				set status = "2"
				where id="'.$order_id.'"');
			}

			@header('location: ./?action=payout');
			exit;

		}elseif($button == 'Перенести в "Не обработанные"'){
			$check_arr = $_POST['check'];

			foreach($check_arr as $order_id){
            	$base->exec('update '.$var['base_tab_prefix'].'count_history
				set status = "0"
				where id="'.$order_id.'"');
			}

			@header('location: ./?action=payout');
			exit;
		}
	}

	$txt .= showMenu('', '',
		array(
		'Не обработанные' => '/administrator/?action=payout&status=0',
		'Обработанные' => '/administrator/?action=payout&status=1',
		'Отклоненные' => '/administrator/?action=payout&status=2'
		), 'navigation_sub_menu', 'td', false
	);

	$txt .= '
	<form id="form" action="'.$_SERVER['REQUEST_URI'].'" class="form_" method="post">
	';

    $txt .= '
	<table class="list">
	 <tr class="head">
	  <td width="1px"><input type="checkbox" onclick="select_checks(this)"></td>
	  <td>Дата</td>
	  <td>Пользователь</td>
	  <td>Сумма</td>
	  <td>Комментарий</td>
	 </tr>
	';

	$history = $base->exec('select id, user_id, amount, text, dataadd
	from '.$var['base_tab_prefix'].'count_history
	where trans_type = "0"
	and status = "'.$status.'"
	and text like "%Яндекс.Деньги%"
	order by dataadd desc');

	if(count($history) != 0){
		foreach($history as $history){
			$user = $base->exec('select id, email, hash
			from '.$var['base_tab_prefix'].'users
			where id = "'.$history['user_id'].'"');

			$txt .= '
			<tr>
		     <td><input name="check[]" type="checkbox" value="'.$history['id'].'"></td>
			 <td>'.date2str($history['dataadd']).'</td>
		     <td><a href="/?change_user_hash='.$user[0]['hash'].'">'.$user[0]['email'].'</a></td>
		     <td>'.$history['amount'].'</td>
		     <td>'.$history['text'].'</td>
		    </tr>';

		    $count_rur_cnt += $history['amount'];
	    }

	}else{
        $txt .= '
		<tr>
		 <td colspan="5" align="center">список пуст.</td>
		</tr>
		';
	}

	$txt .= '
	<tr class="footer">
	 <td colspan="3" class="b">Итого:</td>
	 <td colspan="2" class="b">'.str_replace(",", ".", @sprintf("%.2f", $count_rur_cnt)).'</td>
	</tr>
	<tr>
	 <td colspan="5">
	  '.($_GET['status'] == 0 ? '<input name="button" type="submit" value="Отклонить" class="btn">' : '').'
	  '.($_GET['status'] == 0 ? '<input name="button" type="submit" value="Перенести в &quot;Обработанные&quot;" class="btn">' : '').'
	  '.($_GET['status'] == 0 ? '<input name="button" type="submit" value="Экспорт в CSV" class="btn">' : '').'
	  '.($status == 2 ? '<input name="button" type="submit" value="Перенести в &quot;Не обработанные&quot;" class="btn">' : '').'
	 </td>
	</tr>
	';

	$txt .= '
	</table>
	';

	$txt .= '
	</form>
	';

	return $txt;
}
?>