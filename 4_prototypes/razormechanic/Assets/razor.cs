using UnityEngine;
using System.Collections;

public class razor : MonoBehaviour {

    float currentRotation = 0.0f;
    float rotSpeed = 100.0f;

	// Use this for initialization
	void Start () {
	
	}

    Vector3 oldPos = new Vector3();
	
	// Update is called once per frame
	void Update () {
        oldPos = transform.position;

        Ray r = Camera.main.ScreenPointToRay(new Vector3(Input.mousePosition.x, Input.mousePosition.y, 1));
        RaycastHit hit = new RaycastHit();
        Physics.Raycast(r, out hit);
        transform.position = hit.point + new Vector3(0f, 0.5f, 0f);

        if (Input.GetKey(KeyCode.F))
        {
            ForcePlacement();
        }
        else
        {
            CalculateRotation();
        }

        if (Input.GetKey(KeyCode.Space))
        {
            Camera.main.transform.position += new Vector3(Input.GetAxisRaw("Mouse X"), Input.mouseScrollDelta[1], Input.GetAxisRaw("Mouse Y"));
        }

        currentRotation += Time.deltaTime * rotSpeed * Input.mouseScrollDelta[1];
	}

    Vector3 movement;

    void CalculateRotation()
    {
        movement = (transform.position - oldPos);

        float res = -Mathf.Atan2(movement.z, movement.x) / Mathf.PI * 180f + 90f;

        Debug.Log(res);

        if (movement.x != 0f && movement.z != 0f)
        {
            transform.eulerAngles = new Vector3(0f, Mathf.LerpAngle(transform.eulerAngles.y, res, rotationDelay), 0f);
        }
    }

    public float rotationDelay = 0.1f;

    void OnGUI()
    {
        rotationDelay = GUI.HorizontalSlider(new Rect(10, 10, 200, 10), rotationDelay, 0f, 1f);
        GUI.Label(new Rect(10, 40, 200, 30), rotationDelay.ToString());
    }

    void ForcePlacement()
    {
        transform.eulerAngles = new Vector3(0f, currentRotation, 0f);
    }

    void OnDrawGizmos()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawRay(Vector3.one, new Vector3(movement.x, 0.0f, movement.z));
    }
}
