
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;
use std::io::Write;

#[derive(Debug)]
struct Game {
    duration: f64,
    distance: f64,
}

fn main() -> io::Result<()> {

    let file_name = get_file_name();
    let path = Path::new(&file_name);
    let file = File::open(&path)?;
    let reader = io::BufReader::new(file);

    let mut information: Vec<Vec<String>> = Vec::new();
    // let mut race_record_vec: Vec<String> = Vec::new();
    let mut race_record: String = String::new();

    for line in reader.lines() {
        let line = line?;
        let parts: Vec<String> = line.split_whitespace().map(|s| s.to_string()).collect();

        race_record = parts[1].to_string();
        for i in 2..parts.len(){
            race_record.push_str(&parts[i]);
        }

        println!("{}", race_record);
        

        information.push(vec![parts[0].to_string(), race_record]);
    }

    for info in &information{
        println!("{:?}", info);
    }

    let mut games: Vec<Game> = Vec::new();

    for i in 1..(information[0].len()){

        println!("{} {}", information[0][i], information[1][i]);

        let game = Game {
            duration: information[0][i].parse::<f64>().unwrap(),
            distance: information[1][i].parse::<f64>().unwrap(),
        };

        games.push(game);

    }

    // Now that the info is parsed, turn it into the relevane output

    let mut record_beaters: Vec<i32> = Vec::new();

    for game in games{

        let lower_time_bound = get_lower_bound(game.duration, game.distance);
        let upper_time_bound = get_upper_bound(game.duration, game.distance);

        let num_beaters = upper_time_bound - lower_time_bound + 1;

        record_beaters.push(num_beaters);

    }

    let mut ways_to_beat_record = 1;
    for beater in record_beaters{
        println!("{}", beater);
        ways_to_beat_record *= beater;
    }

    println!("Ways to beat record: {}", ways_to_beat_record);



    Ok(())

}

fn get_file_name() -> String {

    print!("Enter file name: ");
    io::stdout().flush().unwrap();

    let mut _file_name = String::new();

    io::stdin()
        .read_line(&mut _file_name)
        .expect("Failed to read line");
    
    let _file_name = _file_name.trim_end();
    let _file_path = format!(".\\{}.txt", _file_name);

    println!("File is :{}", _file_path);

    _file_path
}

fn get_lower_bound(time: f64, distance: f64) -> i32 {

    let radical = f64::sqrt(time * time - 4.0 * distance);
    let root = (time - radical) / 2.0;

    let lower_bound = if root != root.ceil() { root.ceil() as i32 } else { (root.ceil() + 1.0) as i32 };

    lower_bound

}

fn get_upper_bound(time: f64, distance: f64) -> i32 {

    let radical = f64::sqrt(time * time - 4.0 * distance);
    let root = (time + radical) / 2.0;

    let upper_bound = if root != root.floor() { root.floor() as i32 } else { (root.floor() - 1.0) as i32 };

    upper_bound

}