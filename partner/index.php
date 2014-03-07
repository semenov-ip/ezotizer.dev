<?php
include('../includes/sql.class.inc');
include('../includes/utilites.inc');
include('../cnf.inc');

if(!$var['user_id']){
	@header('location: /login/?return_page='.urlencode($_SERVER['REQUEST_URI']));
	exit;
}

$base->exec('update '.$var['base_tab_prefix'].'users
set who="partner"
where id="'.$var['user_id'].'"');

$action = get_str_val('action');

if(!$action){
	@header('location: ./?action=link');
	exit;
}

include('../includes/menu.class.inc');
include('../includes/pattern.inc');
include('../includes/menu.inc');
include('../includes/balance.inc');
include('../includes/partner.inc');
include('../includes/invitations.inc');

if($action == 'invitations'){
	$body = show_invitations();

}elseif($action == 'link'){
	$body = show_partner_link();

}elseif($action == 'referals'){
	$body = show_partner_referals();
}

$pattern = Array(
	'pattern'	=> '../patterns/index.html',
	'menu'		=> show_menu(),
	'title'		=> $var['title'],
	'body'		=> show_account_menu().show_navigation_menu().show_partner_menu().$body
);

echo show_pattern($pattern);
?>
