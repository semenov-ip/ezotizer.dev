<?php
function show_administrator_sections(){
  global $base;
  global $var;

  $txt .= '
  <div class="section_menu">
    <a href="./?action=sections&add_section=1"><img src="/i/add.gif" width="16" height="16" align="absmiddle">������� ��������</a>
  </div>
  ';

  $txt .= '
  <table class="list">
   <tr class="head">
    <td>��������</td>
    <td>���� �� ���� ��</td>
    <td>���� �� ���� ���</td>
    <td>������</td>
    </tr>
  ';

	$status = Array(
		0 => '�������������',
		1 => '�������'
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
		 <td colspan="3" align="center">������ ����.</td>
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
			$error = '�� ������� ��������.';
		}

		if(!$error){
			if(!$price){
				$error = '�� ������� ���� �� ����.';
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

					$postok = '��������� ���������.';

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
          $error = '�� ������� ��������� ������. ���������� �����.';
				}
			}
		}
	}

	$txt .= '<h1>���������� ��������</h1>';

	if($error){
		$txt .= '<div class="posterror"><img src="/i/error.gif" width="16" height="16" align="absmiddle"> <b>������:</b> '.$error.'</div>';
	}

	if($postok){
		$txt .= '<div class="postok"><img src="/i/ok.gif" width="16" height="16" align="absmiddle"> '.$postok.'</div>';
	}

	$txt .= '
	<form action="'.$_SERVER['REQUEST_URI'].'" method="post">
	 <table class="form">
      <tr>
       <td>��������</td>
       <td><div style="width: 350px;"><input name="name" type="text" value="'.$name.'" class="inp"></div></td>
      </tr>
      <tr>
       <td>���� �� ���� (������)</td>
       <td>
        <input name="price" type="text" value="'.($price ? $price : '0.00').'" size="10" maxlength="9" class="inp_">
       </td>
      </tr>
			<tr>
				<td>���� �� ���� (���)</td>
				<td>
					<input name="price_cis" type="text" value="'.($price_cis ? $price_cis : '0.00').'" size="10" maxlength="9" class="inp_">
				</td>
      </tr>
      <tr>
       <td></td>
       <td>
        <input name="status" id="status" type="checkbox" value="1"'.($status == 1 ? ' checked' : '').'><label for="status">�������� � ������</label>
       </td>
      </tr>
      <tr>
       <td></td>
       <td>
        <input name="postaddsection" type="hidden" value="post">
        <input type="submit" value="���������" class="btn">
       </td>
      </tr>
	 </table>
	</form>
	';

	return $txt;
}
?>