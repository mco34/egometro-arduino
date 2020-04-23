<?php


session_start();

if( !isset($_SESSION['access_token_analytics']) ){
    Header("Location: " . $callback_uri);
    exit();
}



require 'config.php'; 
require '../google-api-php-client/src/Google/autoload.php';


$client = new Google_Client();
$client->setClientId( $google_client_id );
$client->setClientSecret( $google_client_secret );
$client->setAccessToken( $_SESSION['access_token_analytics'] );
$analytics = new Google_Service_Analytics($client);




try {
    $profiles = array();

    $profiles_aux = $analytics->management_webproperties->listManagementWebproperties("~all");

    $items = $profiles_aux->getItems();
    $total = count($items);
    if( $total ){
        for($i=0; $i<$total; $i++){
            $item = $items[$i];
            $profiles[ $item->id ] = array(
                'name' => $item->name,
                'url' => $item->websiteUrl
            );
        }

        $profiles_aux = $analytics->management_profiles->listManagementProfiles("~all","~all");
        $items = $profiles_aux->getItems();
        $total = count($items);
        if( $total ){
            for($i=0; $i<$total; $i++){
                $item = $items[$i];
                $profiles[ $item->webPropertyId ]['accountId'] = $item->id;
            }
        }
    }




} catch (Exception $e) {

    if( strstr($e->getMessage(), "access token has expired") ){
        Header("Location: " . $callback_uri);
    }
}


?>


<p>
    <b>Acces Token</b>
</p>
<p>
    Pega el código "Acces Token" en el fichero config.php</b>
</p>
<p>
    <textarea readonly="readonly" rows="3" cols="30" onclick="this.select()">$access_token = '<?=$_SESSION['access_token_analytics']?>';</textarea>
</p>

<h1>Perfiles</h1>

<p><a href="<?=$callback_uri?>?force">Iniciar sesión con otra cuenta de google</a></p>

<ul>
<?php foreach( $profiles as $id => $item ): ?>
    <li>
        <a href="get.php?id=<?=$item['accountId']?>"><?=$item['name']?></a>
    </li>
<?php endforeach ?>
</ul>