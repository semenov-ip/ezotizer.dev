<?php
function open_flash_chart_object( $width, $height, $url )
{
	$ie = strstr(getenv('HTTP_USER_AGENT'), 'MSIE');
    $url = urlencode($url);

	if ( $ie ) {
		$txt .= '<div id="flashcontent"></div>';
		$txt .= '<script type="text/javascript" src="/i/swfobject.js"></script>';
		$txt .= '<script type="text/javascript">';
		$txt .= 'var so = new SWFObject("/i/chart.swf", "graph_ie", "'. $width . '", "' . $height . '", "8", "#FFFFFF");';
		$txt .= 'so.addVariable("width", "' . $width . '");';
		$txt .= 'so.addVariable("height", "' . $height . '");';
		$txt .= 'so.addVariable("data", "'. $url . '");';
		$txt .= 'so.addParam("allowScriptAccess", "sameDomain");';
		$txt .= 'so.addParam("wmode", "opaque");';
		$txt .= 'so.write("flashcontent");';
		$txt .= '</script>';
		$txt .= '<noscript>';
	}

	$txt .= '<object classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" codebase="http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0" width="' . $width . '" height="' . $height . '" id="graph_ie" align="middle">';
	$txt .= '<param name="allowScriptAccess" value="sameDomain" />';
	$txt .= '<param name="movie" value="/i/chart.swf?width='. $width .'&height='. $height . '&data='. $url .'" />';
    $txt .= '<param name="quality" value="high" />';
    $txt .= '<param name="bgcolor" value="#FFFFFF" />';
	$txt .= '<param name="wmode" value="opaque" />';
	$txt .= '<embed wmode="opaque" src="/i/chart.swf?data=' . $url .'" quality="high" bgcolor="#FFFFFF" width="'. $width .'" height="'. $height .'" name="graph" align="middle" allowScriptAccess="sameDomain" ';
    $txt .= 'type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" id="graph"/>';
	$txt .= '</object>';

	if ( $ie ) {
		$txt .= '</noscript>';
	}

	return $txt;
}
?>