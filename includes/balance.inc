<?php
function show_balance_history(){
	global $base;
	global $var;

	$minus = '0.00';
	$plus = '0.00';

	$txt .= '<p></p>';

    $txt .= '
    <table class="list">
     <tr class="head">
      <td>����</td>
      <td>������</td>
      <td>������</td>
      <td>������ ��������</td>
      <td>�����������</td>
     </tr>
    ';

    $history = $base->exec('select id, amount, text, trans_type, dataadd, status
	from '.$var['base_tab_prefix'].'count_history
	where
	user_id="'.$var['user_id'].'"
	order by dataadd desc');
	
	if(count($history) != 0){
		foreach($history as $history){
			$txt .= '
			<tr>
		     <td>'.date2str($history['dataadd']).'</td>
		     <td>'.($history['trans_type'] == 1 ? $history['amount'] : '-').'</td>
			 <td>'.($history['trans_type'] == 0 ? $history['amount'] : '-').'</td>
			 <td nowrap>
			 '.(
			 $history['status'] == 0 && $history['robokassa_id'] != 0
			 ? '� ��������� [<a href="./?action=payin&type=robokassa&trans_id='.$history['id'].'">���������</a>]'
			 : ($history['status'] == 0 ? '� ���������' : ($history['status'] == 1 ? '���������' : '��������'))
			 ).'
			 </td>
		     <td>'.$history['text'].'</td>
		    </tr>';

		    if($history['status'] == 1){
		    	if($history['trans_type'] == 1)$minus += $history['amount'];
		    	if($history['trans_type'] == 0)$plus += $history['amount'];
		    }

	    }

	}else{
        $txt .= '
		<tr>
		 <td colspan="5" align="center">������ ����.</td>
		</tr>
		';
	}

    $txt .= '
    <tr class="footer b">
     <td>�����:</td>
     <td>'.str_replace(",", ".", $minus).'</td>
     <td>'.str_replace(",", ".", $plus).'</td>
     <td></td>
     <td></td>
    </tr>
    ';

    $txt .= '</table>';

	return $txt;
}
?>