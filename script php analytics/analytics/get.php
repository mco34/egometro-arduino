<?php


require 'config.php'; 
require '../google-api-php-client/src/Google/autoload.php';



if( isset($access_token) ){
	$client = new Google_Client();
	$client->setClientId( $google_client_id );
	$client->setClientSecret( $google_client_secret );
	$client->setAccessToken( $access_token );
	$analytics = new Google_Service_Analytics($client);



	$result = $analytics->data_realtime->get(
		'ga:' . $_GET["id"],
		'rt:activeUsers',
		array('dimensions' => 'rt:medium')
	);

	if( isset($result->totalsForAllResults['rt:activeUsers']) ){
		$total = $result->totalsForAllResults['rt:activeUsers'];
	}else{
		$total = 33333333333;
	}


	header('Content-Type: application/json');

	$data = array('data'=>$total);

	echo json_encode($data);


}else{
	die('Pega el código "Acces Token" en el fichero config.php');
}