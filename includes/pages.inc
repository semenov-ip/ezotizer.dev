<?php
function show_main_page(){
	global $base;
	global $var;

	$txt .= '
	<table>
	 <tr>
	  <td width="40%" class="left">
	   <p>
	    Тизеры — это наборы небольших блоков: текст + картинка, которые «дразнятся», т.е. завлекают посетителя
	    кликнуть и посмотреть интригующую информацию.
	   </p>

	   <p>
        Плюсы нашей сети - максимальная защита от накруток, геотаргетинг, таргетинг по времени, минимальные цены,
        качественные сайты, большой процент (70%) для вебмастеров, реферальная программа (5%).
	   </p>
	';

	$sites_cnt = $base->exec('select count(id)
	from '.$var['base_tab_prefix'].'sites');

	/*$view_cnt = $base->exec('select sum(view) as view
	from '.$var['base_tab_prefix'].'sites_stat
	where dataadd = "'.$var['date'].'"');

	$click_cnt = $base->exec('select sum(click) as click
	from '.$var['base_tab_prefix'].'sites_stat
	where dataadd = "'.$var['date'].'"');*/

	$view_cnt_30 = $base->exec('select sum(view) as view
	from '.$var['base_tab_prefix'].'sites_stat
	where dataadd >= "'.date("Y-m-d", (strtotime($var['date']) - 2592000)).'"
	and dataadd <= "'.$var['date'].'"');

	$click_cnt_30 = $base->exec('select sum(click) as click
	from '.$var['base_tab_prefix'].'sites_stat
	where dataadd >= "'.date("Y-m-d", (strtotime($var['date']) - 2592000)).'"
	and dataadd <= "'.$var['date'].'"');

	$txt .= '

	   <p>
	    <h1>С момента запуска сети<br/> добавлено <b>'.$sites_cnt[0][0].' '.num2word_end($sites_cnt[0][0], '', 'сайт', 'сайта', 'сайтов').'</b>.</h1>
	    <p></p>
	    <h1>За последние 30 дней зафиксировано <b>'.$view_cnt_30[0]['view'].'&nbsp;'.num2word_end($view_cnt_30[0]['view'], '', 'показ', 'показа', 'показов').',
	    '.$click_cnt_30[0]['click'].'&nbsp;'.num2word_end($click_cnt_30[0]['click'], '', 'клик', 'клика', 'кликов').'</b>.
	    Средний <b>CTR '.str_replace(",", ".", @sprintf("%.2f", (100 / $view_cnt_30[0]['view']) * $click_cnt_30[0]['click'])).' %</b>.
	    </h1>

	   </p>

	  </td>

	  <td width="40%" class="center">
	   <p>
	    <img src="/i/screenshot.jpg" style="border: 1px solid #eeeeee;">
	   </p>

	  </td>

	 </tr>
	</table>
	';

	return $txt;
}

function show_page($eng_name){
	global $base;
	global $var;

	$action = get_str_val('action');

	$page = $base->exec('select eng_name, name, text
	from '.$var['base_tab_prefix'].'pages
	where eng_name = "'.$eng_name.'"');

	if($action == 'edit' && $var['user_admin']){
		$name = $page[0]['name'];
		$text = $page[0]['text'];

		if($_POST['posteditpage'] == 'post'){
			$name = stripslashes(htmlspecialchars(trim(post_str_val('name'))));
			$text = stripslashes(htmlspecialchars(trim(post_str_val('text'))));

			if(!$error){
				$base->exec('update '.$var['base_tab_prefix'].'pages
				set
				name = "'.$name.'",
				text = "'.$text.'"
				where eng_name = "'.$eng_name.'"');

        		$postok = 'изменения сохранены.';
			}
		}

		$txt .= '
		<div class="path">
	 	 <img src="/i/path.gif" width="16" height="16" align="absmiddle">
		 <a href="/">Главная</a> /
		 <a href="/'.$page[0]['eng_name'].'/">'.$page[0]['name'].'</a> /
		 <b>Редактирование страницы</b>
		</div>
		';

		$txt .= '<h1>Редактирование страницы</h1>';

		if($postok){
			$txt .= '<div class="postok"><img src="/i/ok.gif" width="16" height="16" align="absmiddle"> '.$postok.'</div>';
		}

  		$txt .= '
		<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
		 <table class="form" width="97%">
	      <tr>
	       <td width="5%">Заголовок*</td>
	       <td><div style="width: 400px;"><input name="name" type="text" value="'.$name.'" class="inp"></div></td>
	      </tr>
	      <tr>
	       <td>Текст*</td>
	       <td><textarea name="text" style="height: 500px;">'.$text.'</textarea></td>
	      </tr>
	      <tr>
	       <td></td>
	       <td>
	        <input name="posteditpage" type="hidden" value="post"><input type="submit" value="Сохранить" class="btn">
	       </td>
	      </tr>
	     </table>
	    </form>
		';

	}else{
		$txt .= '
		<div class="path">
	 	 <img src="/i/path.gif" width="16" height="16" align="absmiddle">
		 <a href="/">Главная</a> /
		 <b>'.$page[0]['name'].'</b>
		</div>
		';

		if($var['user_admin']){
			$txt .= '
			<div class="section_menu_">
			 <a href="./?action=edit"><img src="/i/edit.gif" width="16" height="16" align="absmiddle">Редактировать страницу</a>
			</div>
			';
		}

		$txt .= '<h1>'.$page[0]['name'].'</h1>';
		$txt .= '<p></p>';

		$page[0]['text'] = html_entity_decode($page[0]['text']);
		$page[0]['text'] = str_replace("\n", '<br/>', $page[0]['text']);

		$txt .= '<p>'.$page[0]['text'].'</p>';
	}

	return $txt;
}
?>