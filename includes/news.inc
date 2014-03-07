<?php
function show_news(){
	global $base;
	global $var;

	$txt .= '
	<div class="path">
 	 <img src="/i/path.gif" width="16" height="16" align="absmiddle">
	 <a href="/">Главная</a> /
	 <b>Новости</b>
	</div>
	';

	if($var['user_admin']){
		$txt .= '
		<div class="section_menu_">
		 <a href="./?action=add"><img src="/i/add.gif" width="16" height="16" align="absmiddle">Создать новость</a>
		</div>
		';
	}

	$txt .= '<h1>Новости</h1>';
	$txt .= '<p></p>';

	$news = $base->exec('select id, name, text, dataadd
	from '.$var['base_tab_prefix'].'news
	'.(!$var['user_admin'] ? 'where status = "1"' : '').'
	order by dataadd desc, id desc
	limit 25');

	$txt .= '
	<table class="list">
	';

	if(count($news) > 0){
		$num = 1;

		foreach($news as $news){
        	$txt .= '
        	<tr'.($num?' class="even"':'').'>
        	 <td>
        	  <div class="name">'.($var['user_admin'] ? '<a href="./?action=edit&news_id='.$news['id'].'">'.$news['name'].'</a>' : $news['name']).'</div>
        	  <div class="date">'.date2str($news['dataadd']).'</div>
        	  <div class="text">'.$news['text'].'</div>
        	 </td>
        	</tr>
        	';

        	$num = ($num *= -1) + 1;
		}

	}else{
		$txt .= '
		 <tr>
		  <td colspan="2" align="center">список пуст.</td>
		 </tr>
		';
	}

	$txt .= '
	</table>
	';

	return $txt;
}

function show_news_add(){
	global $base;
	global $var;

	$news_id = get_int_val('news_id');

	$news = $base->exec('select name, text, dataadd, status
	from '.$var['base_tab_prefix'].'news
	where id = "'.$news_id.'"');

	if(count($news) == 1){
		$name = $news[0]['name'];
		$text = $news[0]['text'];
		$dataadd = $news[0]['dataadd'];
		$status = $news[0]['status'];
	}

	if($_POST['postnewnews'] == 'post'){
		$name = stripslashes(htmlspecialchars(trim(post_str_val('name'))));
		$text = stripslashes(htmlspecialchars(trim(post_str_val('text'))));
        $dataadd = date("Y-m-d", strtotime(post_str_val('dataadd')));
        $status = post_int_val('status');

        if(!$name){
        	$error = 'не указан заголовок.';
        }

        if(!$error){
        	if($news_id){
                $base->exec('update '.$var['base_tab_prefix'].'news
                set
                name = "'.$name.'",
                text = "'.$text.'",
                dataadd = "'.$dataadd.'",
                status = "'.$status.'"
                where id = "'.$news_id.'"');

                $postok = 'Изменения сохранены.';

        	}else{
	        	$base->exec('insert into '.$var['base_tab_prefix'].'news
	        	(
	        	name,
	        	text,
	        	dataadd,
	        	status
	        	)values(
	        	"'.$name.'",
	        	"'.$text.'",
	        	"'.$dataadd.'",
	        	"'.$status.'"
	        	)');

	        	$id = $base->new_id();
				if($id){
					@header('location: /news/');
					exit;

				}else{
					$error = 'не удалось создать новость. Попробуйте позже.';
				}
			}
        }
	}

	$txt .= '
	<div class="path">
 	 <img src="/i/path.gif" width="16" height="16" align="absmiddle">
	 <a href="/">Главная</a> /
	 <a href="/news/">Новости</a> /
	 <b>Создание новости</b>
	</div>
	';

    $txt .= '<h1>Создание новости</h1>';

    if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>Ошибка:</b> '.$error.'</div>';
	}

	if($postok){
		$txt .= '<div class="postok"><img src="/i/ok.gif" width="16" height="16" align="absmiddle"> '.$postok.'</div>';
	}

    $txt .= '
	<script language="javascript" type="text/javascript" src="/i/calendar_ru.js"></script>
	<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
	 <table class="form">
      <tr>
       <td width="25%">Заголовок</td>
       <td><div style="width: 350px;"><input name="name" type="text" value="'.$name.'" maxlength="255" class="inp"></div></td>
      </tr>
      <tr>
       <td>Текст</td>
       <td>
        <div style="width: 350px;"><textarea name="text">'.$text.'</textarea></div>
       </td>
      </tr>
      <tr>
       <td>Дата</td>
       <td><input id="dataadd" name="dataadd" type="text" value="'.date("d.m.Y", ($dataadd ? strtotime($dataadd) : strtotime($var['date']))).'" size="11" maxlength="10" class="inp_" onfocus="this.select();lcs(this)" readonly onclick="event.cancelBubble=true;this.select();lcs(this)"></td>
      </tr>
      <tr>
       <td></td>
       <td><input id="status" name="status" type="checkbox" value="1"'.($status == 1 ? ' checked' : '').'><label for="status">Показать в списке</label></td>
      </tr>
      <tr>
       <td></td>
       <td>
        <input name="postnewnews" type="hidden" value="post"><input type="submit" value="Сохранить" class="btn">
       </td>
      </tr>
     </table>
    </form>
	';

	return $txt;
}
?>