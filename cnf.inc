<?php
setlocale(LC_ALL, 'ru_RU.CP1251');
error_reporting(1);
session_start();

##############################################
## настройки mysql ###########################
##############################################

$mysql_host = 'localhost';						// хост базы mysql
$mysql_base = 'ezotizer';					// имя базы mysql
$mysql_user = 'ezotizer';							// пользователь базы mysql
$mysql_pass = 'Fpz9vayp';							// пароль базы mysql
$var['base_tab_prefix'] = 'adtram_';			// префикс таблиц MySQL

$base = new SQLclass($mysql_host, $mysql_base, $mysql_user, $mysql_pass);
$base->exec('set names cp1251');

##############################################
## переменные ################################
##############################################

$var['datetime'] 	= date("Y-m-d H:i:s", time());
$var['date'] 		= date("Y-m-d", time());
$var['week'] 		= date("W", time());
$var['month'] 		= date("n", time());
$var['year'] 		= date("Y", time());
$var['title'] 		= 'Тизерная сеть TeaserLeader';
$var['title_to_code']	= 'Тизерная сеть';
$var['url'] 		= 'teaserleader.ru';
$var['webmaster_procent']	= 0;          ////////////// размер вознаграждения вебмастера
$var['partner_procent']		= 0; 		////////////// размер партнерского вознаграждения
$var['min_payout_price']	= 50;
$var['max_image_file_size']	= 120;
$var['pay_title'] 	= 'teaserLeader';
$var['wmr_purse'] 	= 'R419080924650';
$var['wmz_purse'] 	= 'Z278910767663';
$var['mrh_login'] 	= 'phpteaser';
$var['mrh_pass1'] 	= 'password1';
$var['mrh_pass2']	= 'password2';

##############################################
## прочее ####################################
##############################################

if($_GET['pid'] && !$_COOKIE['pid']){
	@setcookie('pid', $_GET['pid'], time() + 604800, '/');
}

if($_COOKIE['user_hash'] && !$_SESSION['tmp_user_hash']){
	$_SESSION['user_hash'] = $_COOKIE['user_hash'];
}

if($_SESSION['user_hash']){
	$user = $base->exec('select id, pid, email, who, count_rur, curr_num, top10_login, administrator
	from '.$var['base_tab_prefix'].'users
	where hash="'.htmlspecialchars($_SESSION['user_hash']).'"');

	if(count($user) == 0){
		//unset($_SESSION["user_hash"]);
		//unset($_SESSION["tmp_user_hash"]);

		//@setcookie('user_hash', '', time(), '/');

	}else{
		$base->exec('update '.$var['base_tab_prefix'].'users
		set last_data_auth="'.$var['datetime'].'"
		where id="'.$user[0]['id'].'"');

		$var['user_id'] 			= $user[0]['id'];
		$var['partner_id'] 			= $user[0]['pid'];
		$var['user_email'] 			= $user[0]['email'];
		$var['user_who'] 			= $user[0]['who'];
		$var['user_count_rur'] 		= $user[0]['count_rur'];
		$var['user_curr_num'] 		= $user[0]['curr_num'];
		$var['user_top10_login'] 	= $user[0]['top10_login'];

		if($user[0]['administrator'] == 1){
			$var['user_admin'] 		= 1;
		}
	}
}

if($var['user_admin'] && get_str_val('change_user_hash')){
	$_SESSION['tmp_user_hash'] = $_SESSION['user_hash'];
	$_SESSION['user_hash'] = get_str_val('change_user_hash');

	@header('location: /');
	exit;
}

if($_SESSION["tmp_user_hash"] && get_str_val('return_user_hash')){
	unset($_SESSION["tmp_user_hash"]);
	$_SESSION['user_hash'] = get_str_val('return_user_hash');

	@header('location: /');
	exit;
}

if($_SESSION["tmp_user_hash"] && get_str_val('blocked_user_hash')){
	$base->exec('update '.$var['base_tab_prefix'].'users
	set action = "2"
	where hash = "'.get_str_val('blocked_user_hash').'"');

	@header('location: '.$_SERVER['HTTP_REFERER']);
	exit;
}

if($_SESSION["tmp_user_hash"] && get_str_val('unblocked_user_hash')){
	$base->exec('update '.$var['base_tab_prefix'].'users
	set action = "1"
	where hash = "'.get_str_val('unblocked_user_hash').'"');

	@header('location: '.$_SERVER['HTTP_REFERER']);
	exit;
}

if(getenv('REMOTE_ADDR')){
   	$var['user_ip'] = getenv('REMOTE_ADDR');

}else{
    $var['user_ip'] = getenv('HTTP_X_FORWARDED_FOR');
}

if(!$_COOKIE['teaser_uin']){
	@setcookie('teaser_uin', md5($var['user_ip'].$var['datetime']), time()+604800, '/');
}

if($_GET['logout'] == 1){
	unset($_SESSION["user_hash"]);
	unset($_SESSION["tmp_user_hash"]);
	@setcookie('user_hash', '', time(), '/');
	@header('location: /');
	exit;
}
?>
