<?php
include('../includes/sql.class.inc');
include('../includes/utilites.inc');
include('../cnf.inc');

if(!$var['user_id']){
	@header('location: /login/?return_page='.urlencode($_SERVER['REQUEST_URI']));
	exit;
}

if(!$var['user_admin']){
	@header('location: /');
	exit;
}

$base->exec('update '.$var['base_tab_prefix'].'users
set who="administrator"
where id="'.$var['user_id'].'"');

$action = get_str_val('action');

if(!$action){
	@header('location: ./?action=sites');
	exit;
}

include('../includes/pattern.inc');
include('../includes/menu.class.inc');
include('../includes/menu.inc');
include('../includes/administrator.inc');
include('../includes/administrator_sites.inc');
include('../includes/administrator_teasers.inc');
include('../includes/administrator_tickets.inc');
include('../includes/administrator_payout.inc');
include('../includes/administrator_users.inc');
include('../includes/administrator_sections.inc');
include('../includes/administrator_sending.inc');
include('../includes/campaigns_stat.inc');

if($action == 'sending'){
	$body = show_administrator_sending();

}elseif($action == 'sections' && get_int_val('edit_section_id')){
	$body = show_administrator_add_section();

}elseif($action == 'sections' && get_int_val('add_section')){
	$body = show_administrator_add_section();

}elseif($action == 'sections'){
	$body = show_administrator_sections();

}elseif($action == 'users'){
	$body = show_administrator_users();

}elseif($action == 'payout'){
	$body = show_administrator_payout();

}elseif($action == 'tickets' && get_int_val('show_ticket_id')){
	$body = show_administrator_ticket();

}elseif($action == 'tickets'){
	$body = show_administrator_tickets();

}elseif($action == 'teasers'){
	$body = show_administrator_teasers();

}elseif($action == 'check_teaser_blocked'){
	$body = check_teaser_blocked();

}elseif($action == 'check_teaser_active'){
	$body = check_teaser_active();

}elseif($action == 'editsite' && get_int_val('site_id')){
	$body = show_administrator_site_edit();

}elseif($action == 'sites'){
	$body = show_administrator_sites();

}elseif($action == 'check_mod_active'){
	$body = check_mod_active();

}elseif($action == 'check_mod_blocked'){
	$body = check_mod_blocked();

}elseif($action == 'check_blocked'){
	$body = check_blocked();

}elseif($action == 'check_active'){
	$body = check_active();

}else{

}

$ticket_cnt = $base->exec('select count(id)
from '.$var['base_tab_prefix'].'tickets
where upid = "0"
and admin_status = "1"');

$sub_menu = showMenu('', '',
	array(
	'<img src="/i/site.gif" width="16" height="16" align="absmiddle">Сайты' => '/administrator/?action=sites',
	'<img src="/i/campaign.gif" width="16" height="16" align="absmiddle">Тизеры' => '/administrator/?action=teasers',
	'<img src="/i/user.gif" width="16" height="16" align="absmiddle">Пользователи' => '/administrator/?action=users',
	'<img src="/i/tickets.gif" width="16" height="16" align="absmiddle">Тикеты[('.$ticket_cnt[0][0].')]' => '/administrator/?action=tickets',
	'<img src="/i/payout.gif" width="16" height="16" align="absmiddle">Заявки' => '/administrator/?action=payout',
	'<img src="/i/sections.gif" width="16" height="16" align="absmiddle">Тематики' => '/administrator/?action=sections',
	'<img src="/i/sending.gif" width="16" height="16" align="absmiddle">Рассылка' => '/administrator/?action=sending'
	), 'navigation_sub_menu', 'td', false
);

$pattern = Array(
	'pattern'	=> '../patterns/index.html',
	'menu'		=> show_menu(),
	'title'		=> $var['title'],
	'body'		=> show_account_menu().show_navigation_menu().$sub_menu.$body
);

echo show_pattern($pattern);
?>
