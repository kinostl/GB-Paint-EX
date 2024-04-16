<?php
/* A lazy in/out json db to hold save files for A 1-Bit Jam Game */

/*
ini_set('display_errors', '1');
ini_set('display_startup_errors', '1');
error_reporting(E_ALL);
 */

function handleRequest() {
	$db = new PDO('sqlite:/var/www/db/gb_paint.db');
	header('Content-Type: application/json');

	switch($_SERVER['REQUEST_METHOD']) {
	case 'POST':
		$stmt = $db -> prepare("INSERT INTO saves (extram) VALUES (:extram)");

		$data=file_get_contents('php://input');
		$data=json_decode($data, true);
		$data=json_encode($data);

		$stmt -> bindParam(':extram', $data);

		try{
			$stmt -> execute();
			return json_encode(array(
				"success"=>true,
				"message"=>"extram saved"
			));
		}catch(PDOException $e){
			if($e -> errorInfo[0] === "23000"){
				return json_encode(array(
					"success"=>true,
					"message"=>"extram saved"
				));
			}
			return json_encode(array(
				"success"=>false,
				"message"=>"extram failed",
				"error"=>$e
			));
		}
		break;
	case 'GET':
		$stmt = $db -> prepare("SELECT timestamp, extram FROM saves ORDER BY timestamp LIMIT 1");
		$stmt -> execute();
		$res = $stmt -> fetch(PDO::FETCH_ASSOC);
		if($res){
			$res['extram'] = json_decode($res['extram'], true);
			return json_encode($res);
		}else{
			return json_encode(array(
				"success"=>false,
				"message"=>"could not find extram"
			));
		}
		break;
	}
}

?>

<?= handleRequest() ?>
