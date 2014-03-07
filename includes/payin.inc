<?php
function show_payin(){
	global $base;
	global $var;

	$type = get_str_val('type');

    if($type == 'robokassa'){
    	$txt .= show_payin_robokassa();

    }elseif($type == 'webmoney'){
    	$txt .= show_payin_webmoney();

    }elseif($type == 'sms'){
    	$txt .= show_payin_sms();
    }

	return $txt;
}

function show_payin_robokassa(){
	global $base;
	global $var;

	$currs = Array(
  		'WMRM'	 		=> 'WMR',
  		'WMZM'	 		=> 'WMZ',
  		'WMEM'	 		=> 'WME',
  		'PCR'	 		=> 'Яндекс.Деньги',
  		'MailRuR'		=> 'Деньги@Mail.Ru',
  		'MoneyMailR'	=> 'RUR MoneyMail',
  		'RuPayR'		=> 'RUR RBK Money',
  		'MobileWalletR'	=> 'RUR Личный кабинет Qiwi',
  		'W1R'		 	=> 'RUR Единый Кошелек'
	);

	$trans_id = get_int_val('trans_id');

	if($trans_id){
		$trans = $base->exec('select robokassa_id, robokassa_curr_label, amount
		from '.$var['base_tab_prefix'].'count_history
		where user_id = "'.$var['user_id'].'"
		and id = "'.$trans_id.'"
		and status = "0"');

		if(count($trans) == 1){
        	$show_robokassa_form = true;
		}
	}

	if($show_robokassa_form){
		$crc  = md5("".$var['mrh_login'].":".$trans[0]['amount'].":".$trans[0]['robokassa_id'].":".$var['mrh_pass1'].":Shp_user_email=".$var['user_email']."");

		$txt .='
		<form id="roboform" action="https://merchant.roboxchange.com/Index.aspx" method="post">
		 <table class="form">
		  <tr>
	       <td width="25%">Сумма пополнения, руб.:</td>
	       <td>'.$trans[0]['amount'].'</td>
	      </tr>
	      <tr>
	       <td>Валюта:</td>
	       <td>'.$currs[$trans[0]['robokassa_curr_label']].'</td>
	      </tr>
		  <tr>
	       <td>Описание:</td>
	       <td>Пополнение баланса '.$var['user_email'].'</td>
	      </tr>
	      <tr>
	       <td></td>
	       <td>
	        <input type="hidden" name="MrchLogin" value="'.$var['mrh_login'].'">
	        <input type="hidden" name="OutSum" value="'.$trans[0]['amount'].'">
	        <input type="hidden" name="InvId" value="'.$trans[0]['robokassa_id'].'">
	        <input type="hidden" name="Desc" value="Пополнение баланса '.$var['user_email'].'">
			<input type="hidden" name="SignatureValue" value="'.$crc.'">
			<input type="hidden" name="IncCurrLabel" value="'.$trans[0]['robokassa_curr_label'].'">
			<input type="hidden" name="Email" value="'.$var['user_email'].'">
	        <input type="hidden" name="Culture" value="ru">
	        <input type="hidden" name="Shp_user_email" value="'.$var['user_email'].'">
	        <input type="submit" value="Перейти к оплате" class="btn">
	       </td>
	      </tr>
		 </table>
		</form>
		<script>
         /*document.getElementById(\'roboform\').submit();*/
		</script>
		';


	}elseif($_POST['postrobokassa'] == 'post'){
		$OutSum = post_float_val('OutSum');
		$IncCurrLabel = htmlspecialchars(trim(post_str_val('IncCurrLabel')));

		$trans = $base->exec('select robokassa_id, amount
		from '.$var['base_tab_prefix'].'count_history
		where user_id = "'.$var['user_id'].'"
		and amount = "'.$OutSum.'"
		and robokassa_curr_label = "'.$IncCurrLabel.'"
		and trans_type = "1"
		and status = "0"');

		if(count($trans) == 0){
			$trans_cnt = $base->exec('select count(id)
			from '.$var['base_tab_prefix'].'count_history
			where trans_type = "1"');

			$inv_id = $trans_cnt[0][0] + 1;

			$base->exec('insert into '.$var['base_tab_prefix'].'count_history
			(
	        robokassa_id,
			robokassa_curr_label,
			user_id,
	        amount,
	        text,
	        trans_type,
	        dataadd,
	        status
			)values(
	        "'.$inv_id.'",
	        "'.$IncCurrLabel.'",
			"'.$var['user_id'].'",
			"'.$OutSum.'",
			"Пополнение баланса (Robokassa: '.$currs[$IncCurrLabel].')",
			"1",
			"'.$var['datetime'].'",
			"0"
	        )');

	    }else{
           	$inv_id = $trans[0]['robokassa_id'];
	    }

		$crc  = md5("".$var['mrh_login'].":$OutSum:$inv_id:".$var['mrh_pass1'].":Shp_user_email=".$var['user_email']."");

		$txt .='
		<form id="roboform" action="https://merchant.roboxchange.com/Index.aspx" method="post">
		 <table class="form">
		  <tr>
	       <td width="25%">Сумма пополнения, руб.:</td>
	       <td>'.$OutSum.'</td>
	      </tr>
	      <tr>
	       <td>Валюта:</td>
	       <td>'.$currs[$IncCurrLabel].'</td>
	      </tr>
		  <tr>
	       <td>Описание операции:</td>
	       <td>Пополнение баланса '.$var['user_email'].'</td>
	      </tr>
	      <tr>
	       <td></td>
	       <td>
	        <input type="hidden" name="MrchLogin" value="'.$var['mrh_login'].'">
	        <input type="hidden" name="OutSum" value="'.$OutSum.'">
	        <input type="hidden" name="InvId" value="'.$inv_id.'">
	        <input type="hidden" name="Desc" value="Пополнение баланса '.$var['user_email'].'">
			<input type="hidden" name="SignatureValue" value="'.$crc.'">
			<input type="hidden" name="IncCurrLabel" value="'.$IncCurrLabel.'">
			<input type="hidden" name="Email" value="'.$var['user_email'].'">
	        <input type="hidden" name="Culture" value="ru">
	        <input type="hidden" name="Shp_user_email" value="'.$var['user_email'].'">
	        <input type="submit" value="Перейти к оплате" class="btn">&nbsp;<input type="button" value="Вернуться" onclick="history.back(-1);" class="btn">
	       </td>
	      </tr>
		 </table>
		</form>
		<script>
         /*document.getElementById(\'roboform\').submit();*/
		</script>
		';

	}else{
		$txt .='
		<form action="'.$_POST['REQUEST_URI'].'" method="post">
		 <table class="form">
		  <tr>
	       <td width="25%">Сумма, руб.</td>
	       <td>
			<input name="OutSum" type="text" value="300" class="inp_">
			<div class="dsc">Указанная сумма будет зачислена на ваш баланс.</div>
		   </td>
	      </tr>
	      <tr>
	       <td>Валюта</td>
	       <td>
	        <select name="IncCurrLabel">
			 <option value="WMRM">WMR </option>
			 <option value="WMZM">WMZ </option>
			 <option value="WMEM">WME </option>
	 		 <option value="PCR">Яндекс.Деньги </option>
	 		 <option value="MailRuR">Деньги@Mail.Ru </option>
			 <option value="MoneyMailR">RUR MoneyMail </option>
			 <option value="RuPayR">RUR RBK Money </option>
			 <option value="MobileWalletR">RUR Личный кабинет Qiwi </option>
			 <option value="W1R">RUR Единый Кошелек </option>
		    </select>
	       </td>
	      </tr>
	      <tr>
	       <td></td>
	       <td>
	        <input name="postrobokassa" type="hidden" value="post">
	        <input type="submit" value="Оплатить" class="btn">
	       </td>
	      </tr>
		 </table>
		</form>
		';
	}

	return $txt;
}


function show_payin_webmoney(){
	global $base;
	global $var;

	$txt .='
	<form action="https://merchant.webmoney.ru/lmi/payment.asp" method="post">
	 <table class="form">
	  <tr>
       <td width="25%">Сумма</td>
       <td>
        <input type="hidden" name="LMI_PAYMENT_DESC" value="'.$var['pay_title'].' - пополнение баланса - '.$var['user_email'].'">
        <input type="hidden" name="user_email" value="'.$var['user_email'].'">
		<input name="LMI_PAYMENT_AMOUNT" type="text" value="300" class="inp_">
        <select name="LMI_PAYEE_PURSE">
 		 <option value="PCR">Яндекс.Деньги </option>
	    </select>
       </td>
      </tr>
      <tr>
       <td></td>
       <td><input type="submit" value="Оплатить" class="btn"></td>
      </tr>
	 </table>
	</form>
	';

	return $txt;
}
?>