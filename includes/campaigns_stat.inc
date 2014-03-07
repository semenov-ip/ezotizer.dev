<?php
	function campaignsStatExtratc($campaign, $dbName, $idElement){
		global $base;
		global $var;
		
		if( isset($_POST['date_start']) && isset($_POST['date_end']) ){
			$date_start = htmlspecialchars(trim($_POST['date_start']));
			$date_end = htmlspecialchars(trim($_POST['date_end']));

			$begin_day[0]['day'] = date("Y-m-d", strtotime($date_start));
			$contDay = ceil((strtotime($date_end) - strtotime($begin_day[0]['day'])) / 86400) + 1;

			for($i = 1; $i <= $contDay; $i++){

				$dt = strtotime($date_end) - (86400 * ($contDay - $i));

				$dataSearchArr[] = date("Y", $dt)."-".date("m", $dt)."-".date("d", $dt);
			}

			foreach($campaign as $oneCampany){
				$oneCampany['view'] = 0;
				$oneCampany['click'] = 0;
				$oneCampany['ctr'] = 0;
				$oneCampany['count_rur'] = 0;
				$campaignStatistics[$oneCampany['id']] = $oneCampany;

				foreach($dataSearchArr as $dataSearch){

					$stat = $base->exec('select view, click, count_rur, dataadd
						from '.$var['base_tab_prefix'].$dbName.'
						where '.$idElement.' = "'.$oneCampany['id'].'"
						and dataadd="'.$dataSearch.'"');

					if( !empty($stat) ){
						if( count($stat) > 1 ){
							foreach($stat as $arrStat){
								$campaignStatistics[$oneCampany['id']]['view'] += $stat[0]['view'];
								$campaignStatistics[$oneCampany['id']]['click'] += ($stat[0]['click'] ? $stat[0]['click'] : 0);
								$campaignStatistics[$oneCampany['id']]['count_rur'] += round(($stat[0]['count_rur'] ? $stat[0]['count_rur'] : 0.00), 2);
							}
						}else{
							$campaignStatistics[$oneCampany['id']]['view'] += $stat[0]['view'];
							$campaignStatistics[$oneCampany['id']]['click'] += ($stat[0]['click'] ? $stat[0]['click'] : 0);
							$campaignStatistics[$oneCampany['id']]['count_rur'] += round( ($stat[0]['count_rur'] ? $stat[0]['count_rur'] : 0.00), 2);
						}
					}
				}
			}
			foreach( $campaignStatistics as $key=>$val ){
				$view = $val['view'];
				$click = $val['click'];
				$campaignStatistics[$key]['ctr'] = str_replace(",", ".", @sprintf("%.2f", (100 / $view) * $click));
			}
		} else {
			foreach($campaign as $oneCampany){
        $stat = $base->exec('select sum(view) as view, sum(click) as click, sum(count_rur) as count_rur
            from '.$var['base_tab_prefix'].$dbName.'
            where '.$idElement.' = "'.$oneCampany['id'].'"');

				$view = ($stat[0]['view'] ? $stat[0]['view'] : 0);
				$click = ($stat[0]['click'] ? $stat[0]['click'] : 0);

				$oneCampany['view'] = $view;
				$oneCampany['click'] = $click;
				$oneCampany['ctr'] = str_replace(",", ".", @sprintf("%.2f", (100 / $view) * $click));
				$oneCampany['count_rur'] = ($stat[0]['count_rur'] ? $stat[0]['count_rur'] : 0.00);

				$campaignStatistics[$oneCampany['id']] = $oneCampany;
			}
		}
		return $campaignStatistics;
	}