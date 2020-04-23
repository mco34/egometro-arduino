<?php


session_start();
    
if( isset($_GET['force']) ){
    session_destroy();
}

if( isset($_SESSION['access_token_analytics']) ){
    header('Location: list.php');
    exit();
}


require 'config.php'; 
require '../google-api-php-client/src/Google/autoload.php';




$client = new Google_Client();
$client->setClientId( $google_client_id );
$client->setClientSecret( $google_client_secret );
$client->addScope(Google_Service_Analytics::ANALYTICS_READONLY);
$client->setAccessType('offline');
$client->setApprovalPrompt('force');
$client->setRedirectUri($callback_uri);

if (isset($_GET['code'])) {
    $client->authenticate($_GET['code']);
    $_SESSION['access_token_analytics'] = $client->getAccessToken();
    header('Location: list.php');
    exit();
}


if( !isset($_SESSION['access_token_analytics']) ){
    $authUrl = $client->createAuthUrl();
    Header('Location: ' . $authUrl);
}