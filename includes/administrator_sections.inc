<?php
function show_administrator_sections(){
  global $base;
  global $var;

  $txt .= '
  <div class="section_menu">
    <a href="./?action=sections&add_section=1"><img src="/i/add.gif" width="16" height="16" align="absmiddle">Создать тематику</a>
  </div>
  ';

  $txt .= '
  <table class="list">
   <tr class="head">
    <td>Название</td>
    <td>Цена за клик РФ</td>
    <td>Цена за клик СНГ</td>
    <td>Статус</td>
    </tr>
  ';

	$status = Array(
		0 => 'Заблокировано',
		1 => 'Активно'
	);

	$sections = $base->exec('select id, name, price, price_cis, status
	from '.$var['base_tab_prefix'].'sections
	order by binary(name) asc');

	if(count($sections) != 0){
		foreach($sections as $section){
			$num = 0;

			$txt .= '
			<tr'.($num?' class="even"':'').'>
			 <td><a href="./?action=sections&edit_section_id='.$section['id'].'">'.$section['name'].'</a></td>
			 <td>'.$section['price'].'</td>
			 <td>'.$section['price_cis'].'</td>
			 <td>'.$status[$section['status']].'</td>
			</tr>
			';

			$num = ($num *= -1) + 1;
		}

	}else{
    	$txt .= '
		<tr>
		 <td colspan="3" align="center">список пуст.</td>
		</tr>
		';
	}

	$txt .= '
	 <tr>
	  <td colspan="3" class="footer">&nbsp;</td>
	 </tr>
	</table>
	';

	return $txt;
}

function show_administrator_add_section(){
	global $base;
	global $var;

	$section_id = get_int_val('edit_section_id');

	if($section_id){
		$section = $base->exec('select name, price,price_cis, status
		from '.$var['base_tab_prefix'].'sections
		where id = "'.$section_id.'"');

		if(count($section) == 1){
			$name = $section[0]['name'];
			$price = $section[0]['price'];
			$price_cis = $section[0]['price_cis'];
			$status = $section[0]['status'];
		}
	}

	if($_POST['postaddsection'] == 'post'){
		$name = stripslashes(post_str_val('name'));
		$price = post_float_val('price');
		$price_cis = post_float_val('price_cis');
		$status = post_float_val('status');

		if(!$name){
			$error = 'не указано название.';
		}

		if(!$error){
			if(!$price){
				$error = 'не указана цена за клик.';
			}
		}
		
		if(!$error){
			if($section_id){
        $base->exec('update '.$var['base_tab_prefix'].'sections
					set name = "'.$name.'",
					price = "'.$price.'",
					price_cis = "'.$price_cis.'",
					status = "'.$status.'"
					where id = "'.$section_id.'"');

					$postok = 'Изменения сохранены.';

			}else{
    			$base->exec('insert into '.$var['base_tab_prefix'].'sections
    			(
    			name,
    			price,
				price_cis,
    			status
    			)values(
    			"'.$name.'",
    			"'.$price.'",
				"'.$price_cis.'"
    			"'.$status.'"
    			)');

    		$id = $base->new_id();

				if($id){
          @header('location: ./?action=sections');
					exit;

				}else{
          $error = 'не удалось сохранить данные. Попробуйте позже.';
				}
			}
		}
	}

	$txt .= '<h1>Добавление тематики</h1>';

	if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>Ошибка:</b> '.$error.'</div>';
	}

	if($postok){
		$txt .= '<div class="postok"><img src="/i/ok.gif" width="16" height="16" align="absmiddle"> '.$postok.'</div>';
	}

	$txt .= '
	<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
	 <table class="form">
      <tr>
       <td>Название</td>
       <td><div style="width: 350px;"><input name="name" type="text" value="'.$name.'" class="inp"></div></td>
      </tr>
      <tr>
       <td>Цена за клик (Россия)</td>
       <td>
        <input name="price" type="text" value="'.($price ? $price : '0.00').'" size="10" maxlength="9" class="inp_">
       </td>
      </tr>
			<tr>
				<td>Цена за клик (СНГ)</td>
				<td>
					<input name="price_cis" type="text" value="'.($price_cis ? $price_cis : '0.00').'" size="10" maxlength="9" class="inp_">
				</td>
      </tr>
      <tr>
       <td></td>
       <td>
        <input name="status" id="status" type="checkbox" value="1"'.($status == 1 ? ' checked' : '').'><label for="status">Показать в списке</label>
       </td>
      </tr>
      <tr>
       <td></td>
       <td>
        <input name="postaddsection" type="hidden" value="post">
        <input type="submit" value="Сохранить" class="btn">
       </td>
      </tr>
	 </table>
	</form>
	';

	return $txt;
}
?>