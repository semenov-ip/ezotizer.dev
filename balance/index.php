<?php
include('../includes/sql.class.inc');
include('../includes/utilites.inc');
include('../cnf.inc');

if(!$var['user_id']){
	@header('location: /login/?return_page='.urlencode($_SERVER['REQUEST_URI']));
	exit;
}

$action = get_str_val('action');

if(!$action){
	@header('location: ./?action=history');
	exit;
}

include('../includes/pattern.inc');
include('../includes/menu.inc');
include('../includes/balance.inc');
include('../includes/payin.inc');
include('../includes/payout.inc');

if($action == 'history'){
	$body = show_balance_history();

}elseif($action == 'payin'){
	$type = get_str_val('type');

	if(!$type){
		@header('location: ./?action=payin&type=webmoney');
		exit;
	}

	$body = show_payin_menu().show_payin();

}elseif($action == 'payout'){
	$type = get_str_val('type');

	if(!$type){
		@header('location: ./?action=payout&type=webmoney');
		exit;
	}

	$body = show_payout_menu().show_payout();
}

$pattern = Array(
	'pattern'	=> '../patterns/index.html',
	'menu'		=> show_menu(),
	'title'		=> $var['title'],
	'body'		=> show_account_menu().show_balance_menu().$body
);

echo show_pattern($pattern);
?>
