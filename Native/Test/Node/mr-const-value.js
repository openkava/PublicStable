FC = require("Fabric").createClient();

cv = FC.MR.createConstValue("Size", 10000);
console.log(cv.produce());

FC.close();
