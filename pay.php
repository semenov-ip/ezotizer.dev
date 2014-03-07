<?php
function page_parser($url){
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_HEADER, 0);
	curl_setopt($ch, CURLOPT_USERAGENT, "User-Agent=Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.8.1.14) Gecko/20080404 Firefox/2.0.0.14"); // мы - обычный юзер
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	$txt = curl_exec($ch);
	curl_close($ch);

	return $txt;
}

if (!$_POST['LMI_PAYER_PURSE']){
	$merchant_text .= 'error:1';
	echo"err:1";exit;
}

if (!$_POST['LMI_SYS_TRANS_NO']){
	$merchant_text .= 'error:2';
	echo"err:2";exit;
}

if (!$_POST['LMI_PAYER_WM']){
	$merchant_text .= 'error:3';
	echo"err:3";exit;
}

if (!$_POST['LMI_SYS_TRANS_DATE']){
	$merchant_text .= 'error:4';
	echo"err:4";exit;
}

if ($_GET['secret_key'] != '057941c21bb91f067488e065f0ba1f38'){
	$merchant_text .= 'error:4';
	echo "err:5"; exit;
}

include('includes/sql.class.inc');
include('includes/utilites.inc');
include('cnf.inc');

$req = $base->exec('select id, count_rur
from '.$var['base_tab_prefix'].'users
where email="'.$_POST['user_email'].'"');

if(count($req) == 1){
	if(substr($_POST['LMI_PAYER_PURSE'], 0, 1) == 'Z'){
		$usd_from_cbr = page_parser('http://cbrates.rbc.ru/tsv/840/'.date("Y/m/d").'.tsv');
		$pos = strrpos($usd_from_cbr, "\t");
		$usd = trim(substr($usd_from_cbr, $pos));

		$_POST['LMI_PAYMENT_AMOUNT'] = str_replace(",", ".", ($_POST['LMI_PAYMENT_AMOUNT'] * $usd));
	}

	$merchant_text .= $_POST['user_email'].' - '.$_POST['LMI_PAYMENT_AMOUNT'];

	$base->exec('update '.$var['base_tab_prefix'].'users
	set
	count_rur="'.str_replace(",", ".", ($req[0]['count_rur'] + $_POST['LMI_PAYMENT_AMOUNT'])).'"
	where email="'.$_POST['user_email'].'"');

	$base->exec('insert into '.$var['base_tab_prefix'].'count_history
	(
	user_id,
	amount,
	text,
	trans_type,
	dataadd,
	status
	)values(
	"'.$req[0]['id'].'",
	"'.$_POST['LMI_PAYMENT_AMOUNT'].'",
	"Пополнение с WebMoney кошелька '.$_POST['LMI_PAYER_PURSE'].''.($usd ? ' (По курсу &mdash; '.$usd.')' : '').'.",
	"1",
	"'.$var['datetime'].'",
	"1"
	)');
}

/*$mfp = @fopen("merchant.log.txt", "a+");
@fwrite($mfp, $merchant_text);
@fclose($mfp);*/
?>