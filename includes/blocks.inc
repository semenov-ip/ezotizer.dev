<?php
function show_blocks(){
    global $base;
    global $var;

    $site_id = get_int_val('site_id');

    $site = $base->exec('select url from '.$var['base_tab_prefix'].'sites
    where user_id = "'.$var['user_id'].'"
    and id = "'.$site_id.'"');

    if(count($site) != 1){
        @header('location: /webmaster/sites/');
        exit;
    }
  
  $txt .= '
    <div class="path2">
     <img src="/i/path.gif" width="16" height="16" align="absmiddle">
     <a href="/webmaster/">Для вебмастера</a> /
     <a href="/webmaster/sites/">Сайты</a> /
     <b>'.$site[0]['url'].'</b>
    </div>';
  
  $min_dataadd = $base->exec('select min(date(dataadd)) as min
    from '.$var['base_tab_prefix'].'campaigns
    where user_id="'.$var['user_id'].'"');
  
  if($min_dataadd[0]['min'] == '') $min_dataadd[0]['min'] = date("d.m.Y", (strtotime($var['date']) - 604800));
  
  $week_day = array(0=>6, 1=>0, 2=>1, 3=>2, 4=>3, 5=>4, 6=>5);
  
  $txt .= '
    <div class="fast_select position_select">
        <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", strtotime($var['date'])).'\', \''.date("d.m.Y", strtotime($var['date'])).'\');">Сегодня</a>
        <div class="space">&nbsp;</div>
        <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", (strtotime($var['date']) - 86400)).'\', \''.date("d.m.Y", (strtotime($var['date']) - 86400)).'\')">Вчера</a>
        <div class="space">&nbsp;</div>
        <a class="js_lnk" onclick="setdate(\''.date(    "d.m.Y", (strtotime($var['date']) - (86400 * $week_day[date("w", strtotime($var['date']))]))   ).'\', \''.date("d.m.Y", strtotime($var['date'])).'\')">Текущая неделя</a>
        <div class="space">&nbsp;</div>
        <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", (strtotime($var['date']) - (86400 * (date("j", strtotime($var['date'])) - 1)))).'\', \''.date("d.m.Y", strtotime($var['date'])).'\')">Текущий месяц</a>
        <div class="space">&nbsp;</div>
        <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", (strtotime($var['date']) - (86400 * ($week_day[date("w", strtotime($var['date']))] + 7)))).'\', \''.date("d.m.Y", (strtotime($var['date']) - (86400 * ($week_day[date("w", strtotime($var['date']))] + 1)))).'\')">Прошлая неделя</a>
        <div class="space">&nbsp;</div>
        <a class="js_lnk" onclick="setdate(\''.date("d.m.Y", strtotime(date("Y", strtotime($var['date'])).'-'.(date("m", strtotime($var['date'])) - 1).'-01')).'\', \''.date("d.m.Y", (strtotime($var['date']) - (86400 * (date("j", strtotime($var['date'])))))).'\')">Прошлый месяц</a>
    </div>
    <script language="javascript" type="text/javascript" src="/i/calendar_ru.js"></script>
    <form id="myform" class="stat_form margin_top_80" method="POST">
      <table width=40% align="right">
        <tr>
          <td width=15%>&nbsp;</td>
          <td width=30%>Фильтр по дататм: </td>
          <td width=15%><input type="text" name="date_start" class="datepicker" value="'.date("d.m.Y", strtotime($min_dataadd[0]['min'])).'" size="14" maxlength="10"></td>
          <td width=5% align=center>&mdash;</td>
          <td width=15%><input type="text" name="date_end" class="datepicker" value="'.date("d.m.Y", strtotime($var['datetime'])).'" size="14" maxlength="10"></td>
          <td width=20% align="right"><input type="submit" name="statisic" value="Применить"></td>
        </tr>
      </table>
    </form>';

    $txt .= '
    <div class="section_menu">
     <a href="./?action=addblock&site_id='.$site_id.'"><img src="/i/add.gif" width="16" height="16" align="absmiddle">Добавить новый блок</a>
    </div>
    ';

    $txt .= '
    <table class="list">
     <tr class="head">
      <td>Название</td>
      <td>Количество объявлений</td>
      <td>Размер изображения</td>
      <td>Показы</td>
      <td>Клики</td>
      <td>CTR</td>
      <td>Доход</td>
      <td></td>
     </tr>
    ';

    $block = $base->exec('select id, name, hor, ver, size, size_x, size_y
    from '.$var['base_tab_prefix'].'blocks
    where user_id = "'.$var['user_id'].'"
    and site_id = "'.$site_id.'"
    order by dataadd desc');

    $campaignStatistics = campaignsStatExtratc($block, 'blocks_stat', 'block_id');

    $view_cnt = 0;
    $click_cnt = 0;

    if(count($campaignStatistics) > 0){
        $num = 0;

        foreach($campaignStatistics as $block){

          $view = $block['view'];
          $click = $block['click'];
          $ctr = $block['ctr'];
          $count_rur = $block['count_rur'];

          $txt .= '
            <tr'.($num ? ' class="even"' : '').'>
             <td><a href="./?action=editblock&site_id='.$site_id.'&block_id='.$block['id'].'">'.$block['name'].'</a></td>
             <td>'.($block['hor'] * $block['ver']).'</td>
             <td>'.$block['size'].'x'.$block['size'].'</td>
             <td>'.$view.'</td>
             <td>'.$click.'</td>
             <td>'.$ctr.'</td>
             <td>'.$count_rur.'</td>
             <td align="right" nowrap>
              <a href="./?action=editblock&site_id='.$site_id.'&block_id='.$block['id'].'"><img src="/i/edit.gif" width="16" height="16" align="absmiddle" alt="Редактировать рекламный блок" title="Редактировать рекламный блок"></a>
              <a href="./?action=remove&block_id='.$block['id'].'"><img src="/i/delete.gif" width="16" height="16" align="absmiddle" alt="Удалить блок" title="Удалить блок" onclick="conf(\'Вы уверены, что хотите удалить блок?\', this); return false;"></a>
             </td>
            </tr>
            ';

            $num = ($num *= -1) + 1;

            $view_cnt += $view;
            $click_cnt += $click;
            $ctr_cnt += $ctr;
            $count_rur_cnt += $count_rur;
        }

    }else{
        $txt .= '
         <tr>
          <td colspan="8" align="center">список пуст.</td>
         </tr>
        ';
    }

    $txt .= '
     <tr>
      <td colspan="3" class="footer b">Итого:</td>
      <td class="footer b">'.$view_cnt.'</td>
      <td class="footer b">'.$click_cnt.'</td>
      <td class="footer b">'.str_replace(",", ".", @sprintf("%.2f", (100 / $view_cnt) * $click_cnt)).'</td>
      <td class="footer b">'.str_replace(",", ".", @sprintf("%.2f", $count_rur_cnt)).'</td>
      <td class="footer"></td>
     </tr>
    </table>
    ';

    return $txt;
}

function show_block_add(){
    global $base;
    global $var;

    $site_id = get_int_val('site_id');
    $block_id = get_int_val('block_id');

    $site = $base->exec('select url from '.$var['base_tab_prefix'].'sites
    where user_id = "'.$var['user_id'].'"
    and id = "'.$site_id.'"');

    if(count($site) != 1){
        @header('location: /webmaster/sites/');
        exit;
    }

    if($block_id){
        $block = $base->exec('select name, block_size_num, block_size_value,
        hor, ver, size, size_x, size_y, position, align,
        font_family, font_size, font_color, font_color_hover, background_color,
        table_border_width, table_border_color, table_border_style,
        image_border_width, image_border_color, image_border_style,
        cell_background, cell_margin, cell_padding,
        cell_border_width, cell_border_color, cell_border_style,
        show_partner_lnk
        from '.$var['base_tab_prefix'].'blocks
        where user_id = "'.$var['user_id'].'"
        and site_id = "'.$site_id.'"
        and id = "'.$block_id.'"');

        if(count($block) != 1){
            @header('location: /webmaster/sites/?action=blocks&site_id='.$site_id.'');
            exit;
        }

        $name = $block[0]['name'];

        $block_size_num = $block[0]['block_size_num'];
        $block_size_value = $block[0]['block_size_value'];

        $hor = $block[0]['hor'];
        $ver = $block[0]['ver'];
        $size = $block[0]['size'];
        $size_x = $block[0]['size'];
        $size_y = $block[0]['size'];
        $position = $block[0]['position'];
        $align = $block[0]['align'];

        $background_color = $block[0]['background_color'];

        $table_border_width = $block[0]['table_border_width'];
        $table_border_color = $block[0]['table_border_color'];
        $table_border_style = $block[0]['table_border_style'];

        $image_border_width = $block[0]['image_border_width'];
        $image_border_color = $block[0]['image_border_color'];
        $image_border_style = $block[0]['image_border_style'];

        $font_family = $block[0]['font_family'];
        $font_size = $block[0]['font_size'];
        $font_color = $block[0]['font_color'];
        $font_color_hover = $block[0]['font_color_hover'];

        $cell_background = $block[0]['cell_background'];
        $cell_margin = $block[0]['cell_margin'];
        $cell_padding = $block[0]['cell_padding'];

        $cell_border_width = $block[0]['cell_border_width'];
        $cell_border_color = $block[0]['cell_border_color'];
        $cell_border_style = $block[0]['cell_border_style'];

        $show_partner_lnk = $block[0]['show_partner_lnk'];
    }


    if($_POST['postnewblock']){
        $name = stripslashes(trim(post_str_val('name')));

        $block_size_num = post_int_val('block_size_num');
        $block_size_value = stripslashes(htmlspecialchars(trim(post_str_val('block_size_value'))));

        $ver = post_int_val('ver');
        $hor = post_int_val('hor');
        $size = post_int_val('size');
        $size_x = post_int_val('size');
        $size_y = post_int_val('size');
        $position = stripslashes(htmlspecialchars(trim(post_str_val('position'))));
        $align = stripslashes(htmlspecialchars(trim(post_str_val('align'))));

        $background_color = stripslashes(htmlspecialchars(trim(post_str_val('background_color'))));

        $table_border_width = stripslashes(htmlspecialchars(trim(post_str_val('table_border_width'))));
        $table_border_color = stripslashes(htmlspecialchars(trim(post_str_val('table_border_color'))));
        $table_border_style = stripslashes(htmlspecialchars(trim(post_str_val('table_border_style'))));

        $image_border_width = stripslashes(htmlspecialchars(trim(post_str_val('image_border_width'))));
        $image_border_color = stripslashes(htmlspecialchars(trim(post_str_val('image_border_color'))));
        $image_border_style = stripslashes(htmlspecialchars(trim(post_str_val('image_border_style'))));

        $font_family = stripslashes(htmlspecialchars(trim(post_str_val('font_family'))));
        $font_size = stripslashes(htmlspecialchars(trim(post_str_val('font_size'))));
        $font_color = stripslashes(htmlspecialchars(trim(post_str_val('font_color'))));
        $font_color_hover = stripslashes(htmlspecialchars(trim(post_str_val('font_color_hover'))));

        $cell_background = stripslashes(htmlspecialchars(trim(post_str_val('cell_background'))));
        $cell_margin = post_int_val('cell_margin');
        $cell_padding = post_int_val('cell_padding');

        $cell_border_width = stripslashes(htmlspecialchars(trim(post_str_val('cell_border_width'))));
        $cell_border_color = stripslashes(htmlspecialchars(trim(post_str_val('cell_border_color'))));
        $cell_border_style = stripslashes(htmlspecialchars(trim(post_str_val('cell_border_style'))));
        
        $show_partner_lnk = (int) $_POST['show_partner_lnk'];

        if(!$error){
            if(!$name){
                $error = 'не указано название блока.';
                $name = false;
            }
        }

        if(!$error){
            if($position != 'left' && $position != 'right' && $position != 'top'){
                $error = 'не выбрано положение изображения относительно текста.';
            }
        }

        if(!$error){
            if($align != 'center' && $align != 'left' && $align != 'right'){
                $error = 'не выбран способ выравнивания объявления.';
            }
        }
        
        if(!$error){
            if($block_id){
              $base->exec('update '.$var['base_tab_prefix'].'blocks
                set
                name = "'.$name.'",
                block_size_num = "'.$block_size_num.'",
                block_size_value = "'.$block_size_value.'",
                hor = "'.$hor.'",
                ver = "'.$ver.'",
                size = "'.$size.'",
                size_x = "'.$size_x.'",
                size_y = "'.$size_y.'",
                position = "'.$position.'",
                align = "'.$align.'",
                font_family = "'.$font_family.'",
                font_size = "'.$font_size.'",
                font_color = "'.$font_color.'",
                font_color_hover = "'.$font_color_hover.'",
                background_color = "'.$background_color.'",
                table_border_width = "'.$table_border_width.'",
                table_border_color = "'.$table_border_color.'",
                table_border_style = "'.$table_border_style.'",
                image_border_width = "'.$image_border_width.'",
                image_border_color = "'.$image_border_color.'",
                image_border_style = "'.$image_border_style.'",
                cell_background = "'.$cell_background.'",
                cell_margin = "'.$cell_margin.'",
                cell_padding = "'.$cell_padding.'",
                cell_border_width = "'.$cell_border_width.'",
                cell_border_color = "'.$cell_border_color.'",
                cell_border_style = "'.$cell_border_style.'",
                show_partner_lnk = "'.$show_partner_lnk.'"
                where id = "'.$block_id.'"
                ');
              $postok = 'изменения сохранены.';
            }else{
              $base->exec('insert into '.$var['base_tab_prefix'].'blocks
                (
                user_id,
                site_id,
                name,
                block_size_num,
                block_size_value,
                hor,
                ver,
                size,
                size_x,
                size_y,
                position,
                align,
                font_family,
                font_size,
                font_color,
                font_color_hover,
                background_color,
                table_border_width,
                table_border_color,
                table_border_style,
                image_border_width,
                image_border_color,
                image_border_style,
                cell_background,
                cell_margin,
                cell_padding,
                cell_border_width,
                cell_border_color,
                cell_border_style,
                show_partner_lnk,
                dataadd
                )values(
                "'.$var['user_id'].'",
                "'.$site_id.'",
                "'.$name.'",
                "'.$block_size_num.'",
                "'.$block_size_value.'",
                "'.$hor.'",
                "'.$ver.'",
                "'.$size.'",
                "'.$size_x.'",
                "'.$size_y.'",
                "'.$position.'",
                "'.$align.'",
                "'.$font_family.'",
                "'.$font_size.'",
                "'.$font_color.'",
                "'.$font_color_hover.'",
                "'.$background_color.'",
                "'.$table_border_width.'",
                "'.$table_border_color.'",
                "'.$table_border_style.'",
                "'.$image_border_width.'",
                "'.$image_border_color.'",
                "'.$image_border_style.'",
                "'.$cell_background.'",
                "'.$cell_margin.'",
                "'.$cell_padding.'",
                "'.$cell_border_width.'",
                "'.$cell_border_color.'",
                "'.$cell_border_style.'",
                "'.$show_partner_lnk.'",
                "'.$var['datetime'].'"
                )');

                $id = $base->new_id();

                if($id){
                    @header('location: /webmaster/sites/?action=editblock&site_id='.$site_id.'&block_id='.$id.'');
                    exit;

                }else{
                    $error = 'не удалось добавить блок. Попробуйте позже.';
                }
            }
        }
    }

    if(!$name){
        $blocks = $base->exec('select count(*)
        from '.$var['base_tab_prefix'].'blocks
        where user_id = "'.$var['user_id'].'"
        and site_id = "'.$site_id.'"');

        $name = 'Блок #'.($blocks[0][0] + 1);
    }

    if(!$background_color)$background_color = '#ffffff';
    if(!$table_border_color)$table_border_color = '#ffffff';
    if(!$image_border_color)$image_border_color = '#ffffff';
    if(!$font_color)$font_color = '#0092c8';
    if(!$font_color_hover)$font_color_hover = '#0092c8';
    if(!$cell_background)$cell_background = '#ffffff';

    $txt .= '
    <div class="path2">
     <img src="/i/path.gif" width="16" height="16" align="absmiddle">
     <a href="/webmaster/">Для вебмастера</a> /
     <a href="/webmaster/sites/">Сайты</a> /
     <a href="/webmaster/sites/?action=blocks&site_id='.$site_id.'">'.$site[0]['url'].'</a> /
     <b>'.($block_id ? $block[0]['name'] : 'Новый блок').'</b>
    </div>';

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
       <td width="25%">Название блока</td>
       <td><div style="width: 250px;"><input name="name" type="text" value="'.$name.'" class="inp"></div></td>
      </tr>
    ';

    $txt .= '
      <tr>
       <td>Ширина блока</td>
       <td>
        <table>
         <tr>
          <td><div style="width: 40px;"><input name="block_size_num" type="text" value="'.$block_size_num.'" class="inp"></div></td>
          <td>&nbsp;</td>
          <td><div style="width: 40px;"><select name="block_size_value" class="select">
          <option value="px"'.($block_size_value == 'px' ? ' selected' : '').'>px </option>
          <option value="%"'.($block_size_value == '%' ? ' selected' : '').'>% </option>
          </select></div>
          </td>
         </tr>
        </table>
       </td>
      </tr>
      <tr>
       <td>Цвет фона блока</td>
       <td>
        <table>
         <tr>
          <td><div style="width: 60px;"><input name="background_color" id="background_color" type="text" value="'.$background_color.'" class="inp" onkeyup="use_color(\'background_color\', this.value)" onchange="use_color(\'background_color\', this.value)"></div></td>
          <td>&nbsp;</td>
          <td valign="center"><div style="background: '.$background_color.';" class="color_selector" id="background_color_" onclick="show_palette(this, \'background_color\')"></div></td>
         </tr>
        </table>
       </td>
      </tr>
      <tr>
       <td>Размер, стиль и цвет<br/>рамки вокруг блока</td>
       <td>
        <table>
         <tr>
          <td><div style="width: 60px;"><select name="table_border_width" class="select">
           <option value="none"'.($table_border_width == 'none' ? ' selected' : '').'>0 px </option>
           <option value="1"'.($table_border_width == 1 ? ' selected' : '').'>1 px </option>
           <option value="2"'.($table_border_width == 2 ? ' selected' : '').'>2 px </option>
           <option value="3"'.($table_border_width == 3 ? ' selected' : '').'>3 px </option>
           <option value="4"'.($table_border_width == 4 ? ' selected' : '').'>4 px </option>
           <option value="5"'.($table_border_width == 5 ? ' selected' : '').'>5 px </option>
           <option value="6"'.($table_border_width == 6 ? ' selected' : '').'>6 px </option>
           <option value="7"'.($table_border_width == 7 ? ' selected' : '').'>7 px </option>
           <option value="8"'.($table_border_width == 8 ? ' selected' : '').'>8 px </option>
           </select></div>
          </td>
          <td>&nbsp;</td>
          <td><div style="width: 90px;"><select name="table_border_style" class="select">
          <option value="solid"'.($table_border_style == 'solid' ? ' selected' : '').'>Сплошная </option>
          <option value="dashed"'.($table_border_style == 'dashed' ? ' selected' : '').'>Пунктир </option>
          <option value="dotted"'.($table_border_style == 'dotted' ? ' selected' : '').'>Точки </option>
          </select></div>
          </td>
          <td>&nbsp;</td>
          <td><div style="width: 60px;"><input name="table_border_color" id="table_border_color" type="text" value="'.$table_border_color.'" class="inp" onkeyup="use_color(\'table_border_color\', this.value)" onchange="use_color(\'table_border_color\', this.value)"></div></td>
          <td>&nbsp;</td>
          <td valign="center"><div style="background: '.$table_border_color.';" class="color_selector" id="table_border_color_" onclick="show_palette(this, \'table_border_color\')"></div></td>
         </tr>
        </table>
       </td>
      </tr>
    ';

    $txt .= '
      <tr>
       <td>Объявлений по горизонтали</td>
       <td><div style="width: 40px;"><select name="hor" class="select">
        <option value="1"'.($hor == 1 ? ' selected' : '').'>1 </option>
        <option value="2"'.($hor == 2 ? ' selected' : '').'>2 </option>
        <option value="3"'.($hor == 3 ? ' selected' : (!$hor ? ' selected' : '')).'>3 </option>
        <option value="4"'.($hor == 4 ? ' selected' : '').'>4 </option>
        <option value="5"'.($hor == 5 ? ' selected' : '').'>5 </option>
        <option value="6"'.($hor == 6 ? ' selected' : '').'>6 </option>
        <option value="7"'.($hor == 7 ? ' selected' : '').'>7 </option>
        <option value="8"'.($hor == 8 ? ' selected' : '').'>8 </option>
       </select></div></td>
      </tr>
      <tr>
       <td>Объявлений по вертикали</td>
       <td><div style="width: 40px;"><select name="ver" class="select">
        <option value="1"'.($ver == 1 ? ' selected' : '').'>1 </option>
        <option value="2"'.($ver == 2 ? ' selected' : '').'>2 </option>
        <option value="3"'.($ver == 3 ? ' selected' : '').'>3 </option>
        <option value="4"'.($ver == 4 ? ' selected' : '').'>4 </option>
        <option value="5"'.($ver == 5 ? ' selected' : '').'>5 </option>
        <option value="6"'.($ver == 6 ? ' selected' : '').'>6 </option>
        <option value="7"'.($ver == 7 ? ' selected' : '').'>7 </option>
        <option value="8"'.($ver == 8 ? ' selected' : '').'>8 </option>
       </select></div></td>
      </tr>
      <tr>
       <td>Размер изображения</td>
       <td>
        <table>
         <tr>
          <td><div style="width: 100px;"><select name="size" class="select">
           <option value="50"'.($size == 50 ? ' selected' : '').'>50x50 px </option>
           <option value="60"'.($size == 60 ? ' selected' : '').'>60x60 px </option>
           <option value="70"'.($size == 70 ? ' selected' : '').'>70x70 px </option>
           <option value="80"'.($size == 80 ? ' selected' : '').'>80x80 px </option>
           <option value="90"'.($size == 90 ? ' selected' : '').'>90x90 px </option>
           <option value="100"'.($size == 100 ? ' selected' : '').'>100x100 px </option>
           <option value="110"'.($size == 110 ? ' selected' : '').'>110x110 px </option>
           <option value="120"'.($size == 120 ? ' selected' : '').'>120x120 px </option>
           <option value="130"'.($size == 130 ? ' selected' : '').'>130x130 px </option>
           <option value="140"'.($size == 140 ? ' selected' : '').'>140x140 px </option>
           <option value="150"'.($size == 150 ? ' selected' : '').'>150x150 px </option>
           </select></div>
          </td>
    ';

    $txt .= '
         </tr>
        </table>
       </td>
      </tr>
      <tr>
       <td>Размер, стиль и цвет<br/>рамки вокруг изображения</td>
       <td>
        <table>
         <tr>
          <td><div style="width: 60px;"><select name="image_border_width" class="select">
           <option value="none"'.($image_border_width == 'none' ? ' selected' : '').'>0 px </option>
           <option value="1"'.($image_border_width == 1 ? ' selected' : '').'>1 px </option>
           <option value="2"'.($image_border_width == 2 ? ' selected' : '').'>2 px </option>
           <option value="3"'.($image_border_width == 3 ? ' selected' : '').'>3 px </option>
           <option value="4"'.($image_border_width == 4 ? ' selected' : '').'>4 px </option>
           <option value="5"'.($image_border_width == 5 ? ' selected' : '').'>5 px </option>
           <option value="6"'.($image_border_width == 6 ? ' selected' : '').'>6 px </option>
           <option value="7"'.($image_border_width == 7 ? ' selected' : '').'>7 px </option>
           <option value="8"'.($image_border_width == 8 ? ' selected' : '').'>8 px </option>
           </select></div>
          </td>
          <td>&nbsp;</td>
          <td><div style="width: 90px;"><select name="image_border_style" class="select">
          <option value="solid"'.($image_border_style == 'solid' ? ' selected' : '').'>Сплошная </option>
          <option value="dashed"'.($image_border_style == 'dashed' ? ' selected' : '').'>Пунктир </option>
          <option value="dotted"'.($image_border_style == 'dotted' ? ' selected' : '').'>Точки </option>
          </select></div>
          </td>
          <td>&nbsp;</td>
          <td><div style="width: 60px;"><input name="image_border_color" id="image_border_color" type="text" value="'.$image_border_color.'" class="inp" onkeyup="use_color(\'image_border_color\', this.value)" onchange="use_color(\'image_border_color\', this.value)"></div></td>
          <td>&nbsp;</td>
          <td valign="center"><div style="background: '.$image_border_color.';" class="color_selector" id="image_border_color_" onclick="show_palette(this, \'image_border_color\')"></div></td>
         </tr>
        </table>
       </td>
      </tr>
      <tr>
       <td>Положение изображения относительно текста</td>
       <td><div style="width: 250px;"><select name="position" class="select">
        <option value="left"'.($position == 'left' ? ' selected' : '').'>Изображение слева от текста </option>
        <option value="right"'.($position == 'right' ? ' selected' : '').'>Изображение справа от текста </option>
        <option value="top"'.($position == 'top' ? ' selected' : '').'>Изображение сверху над текстом </option>
       </select></div></td>
      </tr>
      <tr>
       <td>Шрифт текста</td>
       <td><div style="width: 110px;"><select name="font_family" class="select">
        <option value="inherit"'.($font_family == 'inherit' ? ' selected' : '').'>Как на сайте </option>
        <option value="Arial"'.($font_family == 'Arial' ? ' selected' : '').'>Arial </option>
        <option value="Times"'.($font_family == 'Times'? ' selected' : '').'>Times </option>
        <option value="Verdana"'.($font_family == 'Verdana' ? ' selected' : '').'>Verdana </option>
        <option value="Tahoma"'.($font_family == 'Tahoma' ? ' selected' : '').'>Tahoma </option>
       </select></div></td>
      </tr>
      <tr>
       <td>Размер текста</td>
       <td><div style="width: 110px;"><select name="font_size" class="select">
        <option value="inherit"'.($font_size == 'inherit' ? ' selected' : '').'>Как на сайте </option>
        <option value="9 px"'.($font_size == 9 ? ' selected' : '').'>9 px </option>
        <option value="10px"'.($font_size == '10px' ? ' selected' : '').'>10 px </option>
        <option value="11px"'.($font_size == '11px' ? ' selected' : '').'>11 px </option>
        <option value="12px"'.($font_size == '12px' ? ' selected' : '').'>12 px </option>
        <option value="13px"'.($font_size == '13px' ? ' selected' : '').'>13 px </option>
        <option value="14px"'.($font_size == '14px' ? ' selected' : '').'>14 px </option>
        <option value="15px"'.($font_size == '15px' ? ' selected' : '').'>15 px </option>
        <option value="16px"'.($font_size == '16px' ? ' selected' : '').'>16 px </option>
        <option value="17px"'.($font_size == '17px' ? ' selected' : '').'>17 px </option>
        <option value="18px"'.($font_size == '18px' ? ' selected' : '').'>18 px </option>
        <option value="19px"'.($font_size == '19px' ? ' selected' : '').'>19 px </option>
        <option value="20px"'.($font_size == '20px' ? ' selected' : '').'>20 px </option>
       </select></div></td>
      </tr>
      <tr>
       <td>Цвет текста</td>
       <td>
        <table>
         <tr>
          <td><div style="width: 60px;"><input name="font_color" id="font_color" type="text" value="'.$font_color.'" class="inp" onkeyup="use_color(\'font_color\', this.value)" onchange="use_color(\'font_color\', this.value)"></div></td>
          <td>&nbsp;</td>
          <td valign="center"><div style="background: '.$font_color.';" class="color_selector" id="font_color_" onclick="show_palette(this, \'font_color\')"></div></td>
         </tr>
        </table>
       </td>
      </tr>
      <tr>
       <td>Цвет текста<br/>при наведении курсора</td>
       <td>
        <table>
         <tr>
          <td><div style="width: 60px;"><input name="font_color_hover" id="font_color_hover" type="text" value="'.$font_color_hover.'" class="inp" onkeyup="use_color(\'font_color_hover\', this.value)" onchange="use_color(\'font_color_hover\', this.value)"></div></td>
          <td>&nbsp;</td>
          <td valign="center"><div style="background: '.$font_color_hover.';" class="color_selector" id="font_color_hover_" onclick="show_palette(this, \'font_color_hover\')"></div></td>
         </tr>
        </table>
       </td>
      </tr>
      <tr>
       <td>Цвет подложки<br/>объявления</td>
       <td>
        <table>
         <tr>
          <td><div style="width: 60px;"><input name="cell_background" id="cell_background" type="text" value="'.$cell_background.'" class="inp" onkeyup="use_color(\'cell_background\', this.value)" onchange="use_color(\'cell_background\', this.value)"></div></td>
          <td>&nbsp;</td>
          <td valign="center"><div style="background: '.$cell_background.';" class="color_selector" id="cell_background_" onclick="show_palette(this, \'cell_background\')"></div></td>
         </tr>
        </table>
       </td>
      </tr>
      <tr>
       <td>Размер отступа между<br/>объявлениями</td>
       <td>
        <table>
         <tr>
          <td><div style="width: 40px;"><input name="cell_margin" type="text" value="'.$cell_margin.'" class="inp"></div></td>
          <td>&nbsp;</td>
          <td>px</td>
         </tr>
        </table>
       </td>
      </tr>
      <tr>
       <td>Размер отступа внутри<br/>объявления</td>
       <td>
        <table>
         <tr>
          <td><div style="width: 40px;"><input name="cell_padding" type="text" value="'.$cell_padding.'" class="inp"></div></td>
          <td>&nbsp;</td>
          <td>px</td>
         </tr>
        </table>
       </td>
      </tr>
      <tr>
       <td>Размер, стиль и цвет<br/>рамки вокруг объявления</td>
       <td>
        <table>
         <tr>
          <td><div style="width: 60px;"><select name="cell_border_width" class="select">
           <option value="none"'.($cell_border_width == 'none' ? ' selected' : '').'>0 px </option>
           <option value="1"'.($cell_border_width == 1 ? ' selected' : '').'>1 px </option>
           <option value="2"'.($cell_border_width == 2 ? ' selected' : '').'>2 px </option>
           <option value="3"'.($cell_border_width == 3 ? ' selected' : '').'>3 px </option>
           <option value="4"'.($cell_border_width == 4 ? ' selected' : '').'>4 px </option>
           <option value="5"'.($cell_border_width == 5 ? ' selected' : '').'>5 px </option>
           <option value="6"'.($cell_border_width == 6 ? ' selected' : '').'>6 px </option>
           <option value="7"'.($cell_border_width == 7 ? ' selected' : '').'>7 px </option>
           <option value="8"'.($cell_border_width == 8 ? ' selected' : '').'>8 px </option>
           </select></div>
          </td>
          <td>&nbsp;</td>
          <td><div style="width: 90px;"><select name="cell_border_style" class="select">
          <option value="solid"'.($cell_border_style == 'solid' ? ' selected' : '').'>Сплошная </option>
          <option value="dashed"'.($cell_border_style == 'dashed' ? ' selected' : '').'>Пунктир </option>
          <option value="dotted"'.($cell_border_style == 'dotted' ? ' selected' : '').'>Точки </option>
          </select></div>
          </td>
          <td>&nbsp;</td>
          <td><div style="width: 60px;"><input name="cell_border_color" id="cell_border_color" type="text" value="'.$cell_border_color.'" class="inp" onkeyup="use_color(\'cell_border_color\', this.value)" onchange="use_color(\'cell_border_color\', this.value)"></div></td>
          <td>&nbsp;</td>
          <td valign="center"><div style="background: '.$cell_border_color.';" class="color_selector" id="cell_border_color_" onclick="show_palette(this, \'cell_border_color\')"></div></td>
         </tr>
        </table>
       </td>
      </tr>
      <tr>
       <td>Выравнивание объявления</td>
       <td><div style="width: 150px;"><select name="align" class="select">
        <option value="center"'.($align == 'center' ? ' selected' : '').'>По центру </option>
        <option value="left"'.($align == 'left' ? ' selected' : (!$align && !$block_id ? 'selected ' : '')).'>По левому краю </option>
        <option value="right"'.($align == 'right' ? ' selected' : '').'>По правому краю </option>
       </select></div></td>
      </tr>
      <tr>
       <td></td>
       <td><input type="hidden" name="show_partner_lnk" id="show_partner_lnk" value="0"></td>
      </tr>
      <tr>
       <td></td>
       <td>
        <input name="postnewblock" type="hidden" value="post"><input type="submit" value="Сохранить" class="btn">
       </td>
      </tr>
     </table>
    </form>
    ';

    if($block_id){
        $txt .= '<h1>Предпросмотр</h1>';

        $txt .= '
        <style>
         #teaser_block_table{font-family: '.$font_family.'; width: '.$block_size_num.''.$block_size_value.'; border-collapse: separate !important; border-spacing: '.$cell_margin.'px; font-size: '.$font_size.'; border: '.($table_border_width == 'none' ? '0' : $table_border_width).'px '.$table_border_style.' '.$table_border_color.'; background-color: '.$background_color.';}
         #teaser_block_table a{color: '.$font_color.' !important;}
         #teaser_block_table a:hover{color: '.$font_color_hover.' !important;}
         #teaser_block_td{border: '.$cell_border_width.'px '.$cell_border_color.' '.$cell_border_style.'; background: '.$cell_background.'; text-align: '.$align.'; vertical-align: top; width: '.round(100 / $hor).'%; padding: '.$cell_padding.'px;}
         #teaser_block_img{'.($position != 'top' ? 'float: '.$position.' !important; ' : '').'width:'.$size.'px; height:'.$size.'px; border: '.($image_border_width == 'none' ? '0' : $image_border_width).'px '.$image_border_style.' '.$image_border_color.'; '.($position == 'left' ? 'margin: 0 '.$cell_padding.'px 0 0' : ($position == 'right' ? 'margin: 0 0 0 '.$cell_padding.'px' : 'margin: 0 0 0 0')).';}
         #teaser_block_partner_lnk{text-align: right;}
         #teaser_block_partner_lnk a{color: inherit !important;}
         #teaser_block_partner_lnk a:hover{color: inherit !important;}
        </style>
        <div class="teaser_preview">'.show_block_preview($hor, $ver, $position, $show_partner_lnk).'</div>
        ';

        $txt .= '<h1>HTML-код</h1>';

        $txt .= "
        <form>
         <table class=\"form\">
          <tr>
           <td>Установите этот код в том месте, где должен выводиться рекламный блок.</td>
          </tr>
          <tr>
           <td>
            <div style=\"width: 100%;\"><textarea readonly><div id=\"teaser_".$block_id."\"><a href=\"http://".$_SERVER['SERVER_NAME']."/\">".$var['title_to_code']."</a></div>
<script type=\"text/javascript\">document.write('<scr'+'ipt type=\"text/jav'+'ascript\" src=\"http://".$_SERVER['SERVER_NAME']."/show/?block_id=".$block_id."&r='+escape(document.referrer)+'&'+Math.round(Math.random()*100000)+'\"></scr'+'ipt>');</script></textarea></div>
           </td>
          </tr>
         </table>
        </form>
        ";

        /*$txt .= "
          <tr>
           <td>
            <textarea readonly></textarea>
           </td>
          </tr>
         </table>
        </form>
        ";*/
    }

    $txt .= '<div id="color_block"></div>';

    return $txt;
}

function show_block_remove(){
    global $base;
    global $var;

    $block_id = get_int_val('block_id');

    $block = $base->exec('select site_id
    from '.$var['base_tab_prefix'].'blocks
    where user_id = "'.$var['user_id'].'"
    and id = "'.$block_id.'"
    ');

    if(count($block) == 1){
        $base->exec('delete from '.$var['base_tab_prefix'].'blocks
        where user_id = "'.$var['user_id'].'"
        and id = "'.$block_id.'"');
    }

    @header('location: /webmaster/sites/?action=blocks&site_id='.$block[0]['site_id'].'');
    exit;
}

function show_block_preview($hor, $ver, $position, $show_partner_lnk){
    global $base;
    global $var;

    $limit = $hor * $ver;

    $teaser = $base->exec('select id, user_id, campaign_id, image, url, text
    from '.$var['base_tab_prefix'].'teasers
    order by last_show asc
    limit '.$limit);

    $txt .= '<table id="teaser_block_table" width="100%" cellspacing="5"><tr>';

    foreach($teaser as $teaser){
        $click_url = html_entity_decode($teaser['url']);

        if($num >= $hor){
            $num = 0;
            $txt .= '</tr><tr>';
            $br = '<br/>';
        }

        $teaser['text'] = str_replace('[-city-]', $ipgeobase['city'], $teaser['text']);
        $teaser['text'] = str_replace("\n", ' ', $teaser['text']);
        $teaser['text'] = str_replace("\r", ' ', $teaser['text']);
        $teaser['text'] = str_replace("  ", ' ', $teaser['text']);
        $teaser['text'] = str_replace("  ", ' ', $teaser['text']);

        $txt .= '<td id="teaser_block_td">';
        $txt .= '<a href="'.$click_url.'" target="_blank" title="'.$teaser['text'].'">';
        $txt .= '<img id="teaser_block_img" src="http://'.$_SERVER['SERVER_NAME'].'/li/'.$teaser['image'].'"></a>';
        $txt .= ''.($position == 'top' ? '<br/>' : '').'';
        $txt .= '<a href="'.$click_url.'" target="_blank" title="'.$teaser['text'].'">'.$teaser['text'].'</a>';
        $txt .= '</td>';

        $num ++;
        $br = '';
    }

    if($show_partner_lnk == 1){ 
        $txt .= '<tr><td colspan="'.$hor.'" id="teaser_block_partner_lnk"><a href="http://'.$_SERVER['SERVER_NAME'].'/?pid='.$var['user_id'].'" target="_blank">Добавить объявление</a></td></tr>';
    }

    $txt .= '</table>';

    return $txt;
}
?>