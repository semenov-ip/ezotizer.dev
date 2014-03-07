<?php
function show_payout(){
	global $base;
	global $var;

    $type = $_GET['type'];

	if($type == 'webmoney'){
    	$txt .= show_payout_webmoney();

    }

	return $txt;
}

function show_payout_webmoney(){
	global $base;
	global $var;

	if($_POST['postpayout'] == 'post'){
		$summ = post_float_val('summ');

		if(!$summ){
			$error = 'не указана сумма.';
		}

		if(!$error){
			if($summ < $var['min_payout_price']){
				$error = 'минимальная сумма для выплаты &mdash; '.$var['min_payout_price'].' руб.';
			}
		}

		if(!$error){
			if($summ > $var['user_count_rur']){
				$error = 'недостаточно средств на балансе.';
			}
		}

		if(!$error){
        	/*if(substr($var['user_curr_num'], 0, 1) == 'Z'){
               	$usd_from_cbr = page_parser('http://cbrates.rbc.ru/tsv/840/'.date("Y/m/d").'.tsv');
				$pos = strrpos($usd_from_cbr, "\t");
				$usd = trim(substr($usd_from_cbr, $pos));

			}*/

            $base->exec('insert into '.$var['base_tab_prefix'].'count_history
			(
			user_id,
			amount,
			text,
			trans_type,
			dataadd,
			status
			)values(
			"'.$var['user_id'].'",
			"'.$summ.'",
			"Выплата на Яндекс.Деньги кошелёк '.$var['user_curr_num'].''.($usd ? ' (По курсу &mdash; '.$usd.')' : '').'.",
			"0",
			"'.$var['datetime'].'",
			"0"
			)');

            $base->exec('update '.$var['base_tab_prefix'].'users
            set count_rur="'.str_replace(",", ".", @sprintf("%.2f", ($var['user_count_rur'] - $summ))).'"
            where id="'.$var['user_id'].'"');

            @header('location: ./');
			exit;
		}
	}

	if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>Ошибка:</b> '.$error.'</div>';
	}

	if($postok){
		$txt .= '<div class="postok"><img src="/i/ok.gif" width="16" height="16" align="absmiddle"> '.$postok.'</div>';
	}

	if($var['user_curr_num'] == ''){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> Необходимо указать номер счета Яндекс.Деньги для выплат на странице &laquo;<a href="/webmaster/settings/">Настройки</a>&raquo;.</div>';

	}else{
		$txt .='
		<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
		 <table class="form">
	      <tr>
	       <td width="25%">Сумма, руб.</td>
	       <td><div style="width: 150px;"><input name="summ" type="text" value="'.($summ ? $summ : '0.00').'" class="inp"></div>
	       </td>
	      </tr>
	      <tr>
	       <td></td>
	       <td><input name="postpayout" type="hidden" value="post"><input type="submit" value="Заказать" class="btn"></td>
	      </tr>
		 </table>
		</form>
		';
	}

	return $txt;
}

?>