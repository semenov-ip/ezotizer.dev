<?php
function show_menu(){
	global $base;
	global $var;

	if(!$var['user_id']){
		/*$txt .= '
		<table align="right">
	     <tr>
	      <td'.($_SERVER['REQUEST_URI'] == '/registration/' ? ' class="action"><b>Регистрация</b>' : (strstr($_SERVER['REQUEST_URI'], '/registration/') ? ' class="action"><b><a href="/registration/">Регистрация</a></b>' : '><a href="/registration/" class="red">Регистрация</a>' )).'</td>
	      <td'.($_SERVER['REQUEST_URI'] == '/login/' ? ' class="action"><b>Вход в систему</b>' : (strstr($_SERVER['REQUEST_URI'], '/login/') ? ' class="action"><b><a href="/login/">Вход в систему</a></b>' : '><a href="/login/" class="red">Вход в систему</a>' )).'</td>
	      <td'.($_SERVER['REQUEST_URI'] == '/agree/' ? ' class="action"><b>Правила</b>' : (strstr($_SERVER['REQUEST_URI'], '/agree/') ? ' class="action"><b><a href="/agree/">Правила</a></b>' : '><a href="/agree/" class="red">Правила</a>' )).'</td>
	      <td'.($_SERVER['REQUEST_URI'] == '/faq/' ? ' class="action"><b>FAQ</b>' : (strstr($_SERVER['REQUEST_URI'], '/faq/') ? ' class="action"><b><a href="/faq/">FAQ</a></b>' : '><a href="/faq/" class="red">FAQ</a>' )).'</td>
	     </tr>
	    </table>
		';*/

	}else{
		/*$txt .= "
		<div id=\"teaser_7\"><a href=\"http://adtram.ru/\">Тизерная сеть adTram</a></div>
		<script type=\"text/javascript\">document.write('<scr'+'ipt type=\"text/jav'+'ascript\" src=\"http://adtram.ru/show/?block_id=7&r='+escape(document.referrer)+'&'+Math.round(Math.random()*100000)+'\"></scr'+'ipt>');</script>
		";*/
	}

	return $txt;
}

function show_account_menu(){
	global $base;
	global $var;

	if($_SESSION['tmp_user_hash']){
		$user = $base->exec('select action
		from '.$var['base_tab_prefix'].'users
		where hash = "'.$_SESSION['user_hash'].'"');

		$txt .= '
		<div class="warning">
		 <img src="/i/warning.gif" class="bigico">
		 Внимание! Вы находитесь в аккаунте пользователя! <a href="/?return_user_hash='.$_SESSION['tmp_user_hash'].'">Вернуться в свой аккаунт...</a><br/>
		';

		if($user[0]['action'] == 2){
			$txt .= '<a href="/?unblocked_user_hash='.$_SESSION['user_hash'].'"><img src="/i/ok.gif" width="16" height="16" align="absmiddle" class="ico">Разблокировать аккаунт</a>';

		}else{
			$txt .= '<a href="/?blocked_user_hash='.$_SESSION['user_hash'].'"><img src="/i/employ.gif" width="16" height="16" align="absmiddle" class="ico">Заблокировать аккаунт</a>';
		}

		$txt .= '
		</div>
		';
	}

	$ticket_cnt = $base->exec('select count(id)
	from '.$var['base_tab_prefix'].'tickets
	where user_id = "'.$var['user_id'].'"
	and upid = "0"
	and status = "1"');

	$txt .= '
	<table class="account_menu">
	 <tr>
	  <td'.($_SERVER['REQUEST_URI'] == '/'.$var['user_who'].'/' ? ' class="active"><b><img src="/i/user.gif" width="16" height="16" align="absmiddle"> '.$var['user_email'].'</b>' : (strstr($_SERVER['REQUEST_URI'], '/'.$var['user_who'].'/') ? ' class="active"><b><a href="/'.$var['user_who'].'/"><img src="/i/user.gif" width="16" height="16" align="absmiddle">'.$var['user_email'].'</a></b>' : ' class="notActive"><a href="/'.$var['user_who'].'/"><img src="/i/user.gif" width="16" height="16" align="absmiddle">'.$var['user_email'].'</a>' )).'</td>
	  <td'.($_SERVER['REQUEST_URI'] == '/balance/' ? ' class="active"><b><img src="/i/balance.gif" width="16" height="16" align="absmiddle"> Баланс</b>' : (strstr($_SERVER['REQUEST_URI'], '/balance/') ? ' class="active"><b><a href="/balance/"><img src="/i/balance.gif" width="16" height="16" align="absmiddle">Баланс</a></b>' : ' class="notActive"><a href="/balance/"><img src="/i/balance.gif" width="16" height="16" align="absmiddle">Баланс</a>' )).' ('.$var['user_count_rur'].' руб.)</td>
	  <td'.($_SERVER['REQUEST_URI'] == '/tickets/' ? ' class="active"><b><img src="/i/tickets.gif" width="16" height="16" align="absmiddle"> Тикеты</b>' : (strstr($_SERVER['REQUEST_URI'], '/tickets/') ? ' class="active"><b><a href="/tickets/"><img src="/i/tickets.gif" width="16" height="16" align="absmiddle">Тикеты</a></b>' : ' class="notActive"><a href="/tickets/"><img src="/i/tickets.gif" width="16" height="16" align="absmiddle">Тикеты</a>' )).' ('.$ticket_cnt[0][0].')</td>
	  <td'.($_SERVER['REQUEST_URI'] == '/news/' ? ' class="active"><b><img src="/i/news.gif" width="16" height="16" align="absmiddle"> Новости</b>' : (strstr($_SERVER['REQUEST_URI'], '/news/') ? ' class="active"><b><a href="/news/"><img src="/i/news.gif" width="16" height="16" align="absmiddle">Новости</a></b>' : ' class="notActive"><a href="/news/"><img src="/i/news.gif" width="16" height="16" align="absmiddle">Новости</a>' )).'</td>
	  <td'.($_SERVER['REQUEST_URI'] == '/top10/' ? ' class="active"><b><img src="/i/top10.gif" width="16" height="16" align="absmiddle"> ТОП10</b>' : (strstr($_SERVER['REQUEST_URI'], '/top10/') ? ' class="active"><b><a href="/top10/"><img src="/i/top10.gif" width="16" height="16" align="absmiddle">ТОП10</a></b>' : ' class="notActive"><a href="/top10/"><img src="/i/top10.gif" width="16" height="16" align="absmiddle">ТОП10</a>' )).'</td>
	  <td'.($_SERVER['REQUEST_URI'] == '/faq/' ? ' class="active"><b><img src="/i/faq.gif" width="16" height="16" align="absmiddle"> FAQ</b>' : (strstr($_SERVER['REQUEST_URI'], '/faq/') ? ' class="active"><b><a href="/faq/"><img src="/i/faq.gif" width="16" height="16" align="absmiddle">FAQ</a></b>' : ' class="notActive"><a href="/faq/"><img src="/i/faq.gif" width="16" height="16" align="absmiddle">FAQ</a>' )).'</td>
	';

	$txt .= '
	  <!--<td width="100%"></td>-->
	  <td class="notActive"><a href="/?logout=1"><img src="/i/exit.gif" width="16" height="16" align="absmiddle">Выйти</a></td>
	 </tr>
	</table>
	';

	return $txt;
}

function show_navigation_menu(){
	global $base;
	global $var;

	$txt .= '
	<table class="navigation_menu">
	 <tr>
	';

	if($var['user_admin']){
		$txt .= '
	  	  <td'.($_SERVER['REQUEST_URI'] == '/advertiser/' ? ' class="active"><b><img src="/i/advertiser.gif" width="16" height="16" align="absmiddle">Для рекламодателя</b>' : (strstr($_SERVER['REQUEST_URI'], '/advertiser/') ? ' class="active"><b><a href="/advertiser/"><img src="/i/advertiser.gif" width="16" height="16" align="absmiddle">Для рекламодателя</a></b>' : ' class="notActive"><a href="/advertiser/"><img src="/i/advertiser.gif" width="16" height="16" align="absmiddle">Для рекламодателя</a>' )).'</td>
		';
	}

	$txt .= '
	  <td'.($_SERVER['REQUEST_URI'] == '/webmaster/' ? ' class="active"><b><img src="/i/webmaster.gif" width="16" height="16" align="absmiddle">Для вебмастера</b>' : (strstr($_SERVER['REQUEST_URI'], '/webmaster/') ? ' class="active"><b><a href="/webmaster/"><img src="/i/webmaster.gif" width="16" height="16" align="absmiddle">Для вебмастера</a></b>' : ' class="notActive"><a href="/webmaster/"><img src="/i/webmaster.gif" width="16" height="16" align="absmiddle">Для вебмастера</a>' )).'</td>
	  <td'.($_SERVER['REQUEST_URI'] == '/partner/' ? ' class="active"><b><img src="/i/partner.gif" width="16" height="16" align="absmiddle">Для партнера</b>' : (strstr($_SERVER['REQUEST_URI'], '/partner/') ? ' class="active"><b><a href="/partner/"><img src="/i/partner.gif" width="16" height="16" align="absmiddle">Для партнера</a></b>' : ' class="notActive"><a href="/partner/"><img src="/i/partner.gif" width="16" height="16" align="absmiddle">Для партнера</a>' )).'</td>
	';

	if($var['user_admin']){
		$txt .= '
	  	<td'.($_SERVER['REQUEST_URI'] == '/administrator/' ? ' class="active"><b><img src="/i/admin.gif" width="16" height="16" align="absmiddle">Администратору</b>' : (strstr($_SERVER['REQUEST_URI'], '/administrator/') ? ' class="active"><b><a href="/administrator/"><img src="/i/admin.gif" width="16" height="16" align="absmiddle">Администратору</a></b>' : ' class="notActive"><a href="/administrator/"><img src="/i/admin.gif" width="16" height="16" align="absmiddle">Администратору</a>' )).'</td>
	  	';
	}

	$txt .= '
	  <td width="100%"></td>
	 </tr>
	</table>
	';

	return $txt;
}

function show_navigation_menu_advertiser(){
	global $base;
	global $var;

	$txt .= '
	<table class="navigation_sub_menu">
	 <tr>
	  <td'.($_SERVER['REQUEST_URI'] == '/advertiser/campaigns/' ? ' class="active"><b><img src="/i/campaign.gif" width="16" height="16" align="absmiddle">Рекламные кампании</b>' : (strstr($_SERVER['REQUEST_URI'], '/advertiser/campaigns/') ? ' class="active"><b><a href="/advertiser/campaigns/"><img src="/i/campaign.gif" width="16" height="16" align="absmiddle">Рекламные кампании</a></b>' : ' class="notActive"><a href="/advertiser/campaigns/"><img src="/i/campaign.gif" width="16" height="16" align="absmiddle">Рекламные кампании</a>' )).'</td>
	  <td'.($_SERVER['REQUEST_URI'] == '/advertiser/settings/' ? ' class="active"><b><img src="/i/setting.gif" width="16" height="16" align="absmiddle">Настройки</b>' : (strstr($_SERVER['REQUEST_URI'], '/advertiser/settings/') ? ' class="active"><b><a href="/advertiser/settings/"><img src="/i/setting.gif" width="16" height="16" align="absmiddle">Настройки</a></b>' : ' class="notActive"><a href="/advertiser/settings/"><img src="/i/setting.gif" width="16" height="16" align="absmiddle">Настройки</a>' )).'</td>
	  <td width="100%"></td>
	 </tr>
	</table>
	';

	return $txt;
}

function show_navigation_menu_webmaster(){
	global $base;
	global $var;

	$txt .= '
	<table class="navigation_sub_menu">
	 <tr>
	  <td'.($_SERVER['REQUEST_URI'] == '/webmaster/sites/' ? ' class="active"><b><img src="/i/site.gif" width="16" height="16" align="absmiddle">Сайты</b>' : (strstr($_SERVER['REQUEST_URI'], '/webmaster/sites/') ? ' class="active"><b><a href="/webmaster/sites/"><img src="/i/site.gif" width="16" height="16" align="absmiddle">Сайты</a></b>' : ' class="notActive"><a href="/webmaster/sites/"><img src="/i/site.gif" width="16" height="16" align="absmiddle">Сайты</a>' )).'</td>
	  <td'.($_SERVER['REQUEST_URI'] == '/webmaster/settings/' ? ' class="active"><b><img src="/i/setting.gif" width="16" height="16" align="absmiddle">Настройки</b>' : (strstr($_SERVER['REQUEST_URI'], '/webmaster/settings/') ? ' class="active"><b><a href="/webmaster/settings/"><img src="/i/setting.gif" width="16" height="16" align="absmiddle">Настройки</a></b>' : ' class="notActive"><a href="/webmaster/settings/"><img src="/i/setting.gif" width="16" height="16" align="absmiddle">Настройки</a>' )).'</td>
	  <td width="100%"></td>
	 </tr>
	</table>
	';

	return $txt;
}

function show_navigation_menu_invitations(){
	global $base;
	global $var;

	$txt .= '
	<table class="navigation_sub_menu">
	 <tr>
	  <td'.($_SERVER['REQUEST_URI'] == '/invitations/?action=free' ? ' class="active"><b><img src="/i/free.gif" width="16" height="16" align="absmiddle">Свободные</b>' : (strstr($_SERVER['REQUEST_URI'], '/invitations/?action=free') ? ' class="active"><b><a href="/invitations/?action=free"><img src="/i/free.gif" width="16" height="16" align="absmiddle">Свободные</a></b>' : ' class="notActive"><a href="/invitations/?action=free"><img src="/i/free.gif" width="16" height="16" align="absmiddle">Свободные</a>' )).'</td>
	  <td'.($_SERVER['REQUEST_URI'] == '/invitations/?action=employed' ? ' class="active"><b><img src="/i/employ.gif" width="16" height="16" align="absmiddle">Занятые</b>' : (strstr($_SERVER['REQUEST_URI'], '/invitations/?action=employed') ? ' class="active"><b><a href="/invitations/?action=employed"><img src="/i/employ.gif" width="16" height="16" align="absmiddle">Занятые</a></b>' : ' class="notActive"><a href="/invitations/?action=employed"><img src="/i/employ.gif" width="16" height="16" align="absmiddle">Занятые</a>' )).'</td>
	  <td width="100%"></td>
	 </tr>
	</table>
	';

	return $txt;
}

function show_balance_menu(){
	global $base;
	global $var;

	$txt .= '
	<table class="navigation_menu">
	 <tr>
	  <td'.($_SERVER['REQUEST_URI'] == '/balance/?action=history' ? ' class="active"><b><img src="/i/balance.gif" width="16" height="16" align="absmiddle">История баланса</b>' : (strstr($_SERVER['REQUEST_URI'], '/balance/?action=history') ? ' class="active"><b><a href="/balance/?action=history"><img src="/i/balance.gif" width="16" height="16" align="absmiddle">История баланса</a></b>' : ' class="notActive"><a href="/balance/?action=history"><img src="/i/balance.gif" width="16" height="16" align="absmiddle">История баланса</a>' )).'</td>
	  <td'.($_SERVER['REQUEST_URI'] == '/balance/?action=payin' ? ' class="active"><b><img src="/i/payin.gif" width="16" height="16" align="absmiddle">Пополнить баланс</b>' : (strstr($_SERVER['REQUEST_URI'], '/balance/?action=payin') ? ' class="active"><b><a href="/balance/?action=payin"><img src="/i/payin.gif" width="16" height="16" align="absmiddle">Пополнить баланс</a></b>' : ' class="notActive"><a href="/balance/?action=payin"><img src="/i/payin.gif" width="16" height="16" align="absmiddle">Пополнить баланс</a>' )).'</td>
	  <td'.($_SERVER['REQUEST_URI'] == '/balance/?action=payout' ? ' class="active"><b><img src="/i/payout.gif" width="16" height="16" align="absmiddle">Заказать выплату</b>' : (strstr($_SERVER['REQUEST_URI'], '/balance/?action=payout') ? ' class="active"><b><a href="/balance/?action=payout"><img src="/i/payout.gif" width="16" height="16" align="absmiddle">Заказать выплату</a></b>' : ' class="notActive"><a href="/balance/?action=payout"><img src="/i/payout.gif" width="16" height="16" align="absmiddle">Заказать выплату</a>' )).'</td>
	  <td width="100%"></td>
	 </tr>
	</table>
	';

	return $txt;
}

function show_payin_menu(){
	global $base;
	global $var;

	$txt .= '
	<table class="navigation_sub_menu">
	 <tr>
	  <td'.($_SERVER['REQUEST_URI'] == '/balance/?action=payin&type=webmoney' ? ' class="active"><img src="/images/app-xFztD.png" width="16" height="16" align="absmiddle"><b>Яндекс.Деньги</b>' : (strstr($_SERVER['REQUEST_URI'], '/balance/?action=payin&type=webmoney') ? ' class="active"><a href="/balance/?action=payin&type=webmoney"><img src="/images/app-xFztD.png" width="16" height="16" align="absmiddle">Яндекс.Деньги</a>' : ' class="notActive"><a href="/balance/?action=payin&type=webmoney"><img src="/images/app-xFztD.png" width="16" height="16" align="absmiddle">Яндекс.Деньги</a>' )).'</td>
	  <td width="100%"></td>
	 </tr>
	</table>
	';

	return $txt;
}

function show_payout_menu(){
	global $base;
	global $var;

	$txt .= '
	<table class="navigation_sub_menu">
	 <tr>
	  <td'.($_SERVER['REQUEST_URI'] == '/balance/?action=payout&type=webmoney' ? ' class="active"><b><img src="/images/app-xFztD.png" width="16" height="16" align="absmiddle">Яндекс.Деньги</b>' : (strstr($_SERVER['REQUEST_URI'], '/balance/?action=payout&type=webmoney') ? ' class="active"><b><a href="/balance/?action=payout&type=webmoney"><img src="/images/app-xFztD.png" width="16" height="16" align="absmiddle">Яндекс.Деньги</a></b>' : ' class="notActive"><a href="/balance/?action=payout&type=webmoney"><img src="/images/app-xFztD.png" width="16" height="16" align="absmiddle">Яндекс.Деньги</a>' )).'</td>
	  <td width="100%"></td>
	 </tr>
	</table>
	';

	return $txt;
}

function show_partner_menu(){
	global $base;
	global $var;

	$txt .= showMenu('', '',
		array(
		'<img src="/i/link.gif" width="16" height="16" align="absmiddle">Реферальная ссылка' => '/partner/?action=link',
		'<img src="/i/referals.gif" width="16" height="16" align="absmiddle">Рефералы' => '/partner/?action=referals',
		'<img src="/i/invitation.gif" width="16" height="16" align="absmiddle">Приглашения (инвайты)' => '/partner/?action=invitations',
		' {100%}' => ''
		), 'navigation_sub_menu', 'td', false
	);

	return $txt;
}
?>