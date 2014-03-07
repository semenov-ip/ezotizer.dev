<?php
include('../includes/sql.class.inc');
include('../includes/utilites.inc');
include('../cnf.inc');

if(!$var['user_id']){
	@header('location: /login/?return_page='.urlencode($_SERVER['REQUEST_URI']));
	exit;
}

$action = get_str_val('action');

include('../includes/pattern.inc');
include('../includes/menu.inc');
include('../includes/tickets.inc');

if($action == 'add'){
	$body = show_ticket_add();

}elseif($action == 'edit' && get_int_val('ticket_id')){
	$body = show_ticket();

}else{
	$body = show_tickets();
}

$pattern = Array(
	'pattern'	=> '../patterns/index.html',
	'menu'		=> show_menu(),
	'title'		=> $var['title'],
	'body'		=> show_account_menu().$body
);

echo show_pattern($pattern);
?>
