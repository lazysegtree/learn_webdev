import "./my_list.css";
import axios from "axios";
import MYListCard from "./my_list_card";

const MyList = ()=> {
    const [list,setList]=useState([]);

    // form data state
    const [details,setDetails]=useState({
        name:"",
        year:"",
        price:"",
        model:"",
        size:""
    })
    // update form value
    const changeHandler= (event)=> {
        const {name,value}=event.target;
        setDetails((prevdetails)=> {
            return {
                ...prevdetails,
                [name]:value
            }
        })
    }
    const url="https://api.restful-api.dev/objects";
    // form submit submit click
    const submitHandler = async(event)=> {
        event.preventDefault();
        try {
            const res=await axios.post(url,{
                "name":details.name,
                "data":{
                    "year":details.year,
                    "price":details.price,
                    "CPU model":details.model,
                    "Hard disk size":details.size
                }
            });
            alert("Successfully Posted");
            console.log("from post request",res);
        } catch(err) {
            console.log(err);
        }
        setDetails({
            name:"",
            year:"",
            price:"",
            model:"",
            size:""
        })
    };
    const get_request=async()=> {
        // var res=await axios.get(url);
        // setList(res.data)
        // console.log("this",res.data);
    }
    get_request();
    
    return <div className="list-container1">
        <div className="get-list-conatiner">
            {list.map((ele,ind)=> <MYListCard key={ind} obj={ele} /> )}
        </div>
        <form >
            <input type="text" placeholder="Name" name="name" value={details.name} onChange={changeHandler}/>
            <input type="number" placeholder="Year" name="year" value={details.year} onChange={changeHandler}/>
            <input type="number" name="price" placeholder="Price" value={details.price} onChange={changeHandler}/>
            <input type="text" placeholder="CPU model" name="model" value={details.model} onChange={changeHandler}/>
            <input type="text" placeholder="Hard Disk Size" name="size" value={details.size} onChange={changeHandler}/>
            <button type="submit" onClick={submitHandler}>Submit</button>
        </form>
    </div>
}

export default MyList;